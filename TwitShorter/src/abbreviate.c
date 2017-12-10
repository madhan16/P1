#include "../includes/StructsAndConsts.h"
#include "../includes/tweetsetup.h"
#include "../includes/abbreviate.h"

/*ABBREVIATION FUNCTIONS*/
/*READS TXT FILE WITH ABBREVIATION WORDS AND PUTS INTO ARRAY OF STRUCTS
  CALLED ABB_TBL*/

/*====================================================
    FUNCTIONS SETTING UP THE ABBREVIATION DATABASE 
  ====================================================*/ 

abb_tbl *abbreviation_setup(int *lines) {
    abb_tbl *abb_list;
    FILE *ifp = fopen(ABB_PATH, "r");
    /*check if connection was successfull*/
    if(ifp == NULL) {
        printf("Could not connect abbreviation file\n");
        exit(EXIT_FAILURE);
    }
    *lines = count_lines_in_file(ifp);
    abb_list = (abb_tbl*)malloc(*lines * sizeof(abb_tbl));
    
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

/*================================================
    FUNCTIONS FOR ABBREVIATING THE TWEET
  ===============================================*/ 
  
/*Returns abbreviated tweet via pointer to array of structs and returns amount of words added*/
int add_abbreviation_to_tweet(twitter_words_tbl *wl, int wl_words, abb_tbl *abb_list, 
                              int abb_words, twitter_words_tbl *wlc) {
    int i, k = 0;
    int result_id = 0;
    for(i = 0; i < wl_words; i++) {
        result_id = analyse_word(wl, wl_words, &i, abb_list, abb_words);
        
        if(result_id == -1)
            wlc[k++] = transfer_word_to_list(wlc, wl[i].word);
        else
            wlc[k++] = transfer_word_to_list(wlc, abb_list[result_id].abb);
    }
    /*where k is amount of words added*/
    return k;
}

int analyse_word(twitter_words_tbl *wl, int wl_words, int *word_index, 
                   abb_tbl *abb_list, int abb_words) {
    int i, j, str_len;
    char curr_word[MAX_WORD_LGT];
    char temp_word[MAX_WORD_LGT * 3];
    strcpy(curr_word, wl[*word_index].word);
    
    for (i = 0; i < abb_words; i++) {
        strcpy(temp_word, curr_word);
        j = 1;
        if(abb_is_multiple_words(abb_list, i, *word_index, wl_words)) {
            for(j = 1; j < abb_list[i].n_words; j++) {
                str_len = strlen(temp_word);
                strcpy(temp_word + str_len, " ");
                strcpy(temp_word + str_len + 1, wl[*word_index + j].word);
            }
        }
        if ((strcmp(temp_word, abb_list[i].word) == 0)) {
            *word_index += (j - 1);
            return i;
        }
    }
    return -1;
}

/*returns 1 if strings contains more than one word and stops from
  reading through array elements*/
int abb_is_multiple_words(abb_tbl *abb_list, int index, int word_index, int wl_words) {
    return abb_list[index].n_words > 1 &&
           abb_list[index].n_words + word_index < wl_words;
}

/*prints abbreviated tweet to file for twitter tagger*/
void print_abbreviated_tweet_to_file(twitter_words_tbl *wlc, int n_words) {
    FILE *ofp = fopen(ABB_WRITE_PATH, "w");
    int i, space_or_not;
    
    for(i = 0; i < n_words; i++) {
        /*if not special signs add space after word*/
        space_or_not = add_space_or_not(wlc[i].word, wlc[i + 1].word);
        
        fprintf(ofp, "%s%s", wlc[i].word, space_or_not == 1 ? " " : "");
    }
    
    fclose(ofp);
}

int add_space_or_not(char *str1, char *str2) {
    char signsWithSpaceAfter[10] = {'!', '?', '.', ',', ')'};
    
    if(in_array(*str2, signsWithSpaceAfter))
        return 0;
    
    return 1;
}
