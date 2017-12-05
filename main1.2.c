#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#define MAX_LINE 200
#define MAX_WORD_LGT 50
#define WORD_LIST_LGT 100

typedef struct word {
    char word[MAX_WORD_LGT];
    int  is_capital;
    int  is_twittertag;
} twitter_words_tbl;

typedef struct abb {
    char word[MAX_WORD_LGT];
    char abb[MAX_WORD_LGT];
    int  n_words;
} abb_tbl;

int in_array(char val, char *arr);
int split_to_words(char *str, twitter_words_tbl *wl);
twitter_words_tbl transfer_word_to_list(twitter_words_tbl *wl, char *word);
void print_word_list(twitter_words_tbl wl);
int word_starts_upper(char *str);
int is_twitter_tag(char *str);

/*abbreviate functions*/
abb_tbl *abbreviation_setup(int *lines);
int count_lines_in_file(FILE *abbfp);
void fill_array_from_file(FILE *abbfp, abb_tbl *arr, int *lines);
abb_tbl read_file_line(FILE *abbfp);
int count_words_in_string(char *str);
void print_abb_list(abb_tbl *arr, int *lines);

/*compress tweet*/
void compress_tweet(twitter_words_tbl *wl, int wl_words, abb_tbl *abb_list, int abb_lines);
void add_abbreviation_to_tweet(twitter_words_tbl *wl, int wl_words, abb_tbl *abb_list, 
                               int abb_lines, twitter_words_tbl *wlc);
int analyse_word(twitter_words_tbl *wl, int last_word_index, int *word_index, 
                   abb_tbl *abb_list, int abb_lines);
int abb_is_multiple_words(abb_tbl *abb_list, int index, int word_index, int last_word_index);


int main(void) {
    int count_twitter_words, abb_tbl_lines;
    abb_tbl *abb_list;
    twitter_words_tbl word_list[WORD_LIST_LGT];
    
    char tweet[] = "Friday night in september as soon as possible is not as going to we are in this laughing out loud for Alaska when Professor Doctor family died as expected";
    
    /*abbreviation setup*/
    abb_list = abbreviation_setup(&abb_tbl_lines);
    
    /*setup tweet*/
    count_twitter_words = split_to_words(tweet, word_list);
    
    /*compress tweet*/
    compress_tweet(word_list, count_twitter_words, abb_list, abb_tbl_lines);
    
    free(abb_list);
    return 0;
}

/*TWEET FUNCTIONS*/
/*TAKES A TWEET STRING FROM MAIN AND SPLITS INTO WORDS IN ARRAY OF
  STRUCTS CALLED TWITTER_WORDS_TBL*/

int split_to_words(char *str, twitter_words_tbl *wl) {
    int i, j = 0, k = 0;
    /*temperary string to store word*/
    char temp[30];
    /*special characters to check for*/
    char signsWithSpaceAfter[10] = {'!', '?', '.', ',', ')'};
    char specialSigns[10] = {'('};
    char twitterTags[10] = {'#', '@', '$', '-', '\''};
    int countWords;
    
    for(i = 0; i <= (strlen(str)); i++) {
        if (isalnum(str[i]) || in_array(str[i], twitterTags)) {
            temp[j++] = str[i];
        }
        else if(str[i] == ' ' || str[i] == '\0') {
            /*complete string*/
            temp[j] = '\0';
            wl[k++] = transfer_word_to_list(wl, temp);
            /*reset element*/
            j = 0;
        }
        /*if sign is in array*/
        else if(in_array(str[i], signsWithSpaceAfter)) {
            /*complete temperary string found before special sign*/
            temp[j] = '\0';
            wl[k++] = transfer_word_to_list(wl, temp);
            j = 0;
            /*add special sign*/
            temp[j++] = str[i];
        }
        else if(in_array(str[i], specialSigns)) {
            temp[j++] = str[i];
            temp[j] = '\0';
            wl[k++] = transfer_word_to_list(wl, temp);
            j = 0;
        }
    }
    
    temp[j] = '\0';
    wl[k++] = transfer_word_to_list(wl, temp);
    
    countWords = k;
    return countWords;
}

/*checks if val is found in array*/
int in_array(char val, char *arr) {
    int i;
    int size = sizeof(arr);
    for(i = 0; i < size; i++) {
        if(arr[i] == val)
            return 1;
    }
    return 0;
}

/*transfers word to word table*/
twitter_words_tbl transfer_word_to_list(twitter_words_tbl *wl, char *word) {
    twitter_words_tbl result;
    strcpy(result.word, word);
    result.is_capital = word_starts_upper(word);
    result.is_twittertag = is_twitter_tag(word);
    
    return result;
}

/*checks wheter string starts uppercase*/
int word_starts_upper(char *str) {
    char ch = str[0];
    if(ch >= 'A' && ch <= 'Z')
        return 1;
    else
        return 0;
}

int is_twitter_tag(char *str) {
    int i;
    char twitterTags[10] = {'#', '@', '$'};
    
    for(i = 0; i < 3; i++) {
        if(twitterTags[i] == str[0])
            return 1;
    }
    return 0;
}

/*can only print one element of array*/
void print_word_list(twitter_words_tbl wl) {
    char *is_upper,
         *is_twittertag;
    
    is_upper = (wl.is_capital ? "yes" : "no");
    is_twittertag = (wl.is_twittertag ? "yes" : "no");

    printf("Word: %-17s Capital: %-4s Twittertag: %s\n", wl.word, is_upper, is_twittertag);
}

/*ABBREVIATION FUNCTIONS*/
/*READS TXT FILE WITH ABBREVIATION WORDS AND PUTS INTO ARRAY OF STRUCTS
  CALLED ABB_TBL*/

abb_tbl *abbreviation_setup(int *lines) {
    abb_tbl *abb_list;
    FILE *ifp = fopen("shortened.txt", "r");
    /*check if connection was successfull*/
    if(ifp == NULL) {
        printf("Could not connect to file\n");
        exit(EXIT_FAILURE);
    }
    *lines = count_lines_in_file(ifp);
    abb_list = (abb_tbl*)malloc((int)lines * sizeof(abb_tbl));
    
    fill_array_from_file(ifp, abb_list, lines);
    fclose(ifp);
    
    return abb_list;
}

/*takes a file points and returns number of lines in file*/
int count_lines_in_file(FILE *abbfp) {
    int lines = 0;
    char temp_str[MAX_LINE];
    
    while(fgets(temp_str, MAX_LINE, abbfp)) {
        lines++;
    }
    rewind(abbfp);
    
    return lines;
}

/*takes a file pointer, array of structs and size of array and fill 
  array of struct from file*/
void fill_array_from_file(FILE *abbfp, abb_tbl *arr, int *lines) {
    int i;
    for(i = 0; i < *lines; i++)
        arr[i] = read_file_line(abbfp);
}

abb_tbl read_file_line(FILE *abbfp) {
    abb_tbl p;
    
    fscanf(abbfp, " %[^:] : %[^\n] ", p.word, p.abb);
    p.n_words = count_words_in_string(p.word);
    
    return p;
}

/*returns number of words in string*/
int count_words_in_string(char *str) {
    int i, count = 0;
    
    for(i = 0; i < strlen(str); i++) {
        if(str[i] == ' ')
            count++;
    }
    return count + 1;
}

/*prints abbreviation list*/
void print_abb_list(abb_tbl *arr, int *lines) {
    int i;
    
    printf("%-4s %-30s %-10s %2s\n", "#", "WORD", "ABB.", "N");
    
    for(i = 0; i < *lines; i++)
        printf("%-4d %-30s %-10s %2d\n", (i + 1), arr[i].word, arr[i].abb, arr[i].n_words);
}

/*COMPRESS TWEET*/
/*MAKES A TWEET MORE READABLE*/

void compress_tweet(twitter_words_tbl *wl, int wl_words, abb_tbl *abb_list, int abb_lines) {
    int i;
    twitter_words_tbl word_list_abbreviated[WORD_LIST_LGT];
    
    add_abbreviation_to_tweet(wl, wl_words, abb_list, abb_lines, word_list_abbreviated);
    
    /*FÅ POINTER TIL ANTAL ORD*/
    for(i = 0; i < 30; i++)
        printf("%s ", word_list_abbreviated[i].word);
    printf("\n");
}

/*returns abbreviated tweet via pointer to array of structs*/
void add_abbreviation_to_tweet(twitter_words_tbl *wl, int wl_words, abb_tbl *abb_list, 
                               int abb_lines, twitter_words_tbl *wlc) {
    int i, k = 0;
    int result_id = 0;
    for(i = 0; i < wl_words; i++) {
        result_id = analyse_word(wl, wl_words, &i, abb_list, abb_lines);
        
        if(result_id == -1)
            wlc[k++] = transfer_word_to_list(wlc, wl[i].word);
        else
            wlc[k++] = transfer_word_to_list(wlc, abb_list[result_id].abb);
    }
}

int analyse_word(twitter_words_tbl *wl, int last_word_index, int *word_index, 
                   abb_tbl *abb_list, int abb_lines) {
    int i, j, str_len;
    char curr_word[MAX_WORD_LGT];
    char temp_word[MAX_WORD_LGT * 3];
    strcpy(curr_word, wl[*word_index].word);
    
    for (i = 0; i < abb_lines; i++) {
        if(abb_is_multiple_words(abb_list, i, *word_index, last_word_index)) {
            strcpy(temp_word, curr_word);
            for(j = 1; j < abb_list[i].n_words; j++) {
                str_len = strlen(temp_word);
                strcpy(temp_word+str_len, " ");
                strcpy(temp_word+str_len+1, wl[*word_index+j].word);
            }
            if ((strcmp(temp_word, abb_list[i].word) == 0)) {
                *word_index += (j - 1);
                return i;
            }
        }
        else if ((strcmp(curr_word, abb_list[i].word) == 0)) {
            return i;
        }
    }
    return -1;
}

/*returns 1 if word is single and ... */
int abb_is_multiple_words(abb_tbl *abb_list, int index, int word_index, int last_word_index) {
    return abb_list[index].n_words > 1 &&
           abb_list[index].n_words + word_index < last_word_index;
}
