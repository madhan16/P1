/*converts a long string into elements in struct, word by word*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#define WORD_NAME_LGT 50
#define WORD_LIST_LGT 100

typedef struct word {
    char word[WORD_NAME_LGT];
    int  is_capital;
    int  is_twittertag;
} word_tbl;

int in_array(char val, char *arr);
int split_to_words(char *str, word_tbl *wl);
word_tbl transfer_word_to_list(word_tbl *wl, char *word);
void print_word_list(word_tbl wl);
int word_starts_upper(char *str);
int is_twitter_tag(char *str);

int main(void) {
    int i, countWords;
    word_tbl word_list[WORD_LIST_LGT];
    char tweet[] = "Our boy @tobygreendj crushing it. $Tesla that's on @mintsyndicate rn! Big up @austinkramer SygtNokBro";
    
    countWords = split_to_words(tweet, word_list);
    
    for(i = 0; i < countWords; i++)
        print_word_list(word_list[i]);
    return 0;
}

int split_to_words(char *str, word_tbl *wl) {
    int i, j = 0, k = 0;
    /*temperary string to store word*/
    char temp[30];
    /*special characters to check for*/
    char signsWithSpaceAfter[10] = {'!', '?', '.', ','};
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

/*transfers word to world table*/
word_tbl transfer_word_to_list(word_tbl *wl, char *word) {
    word_tbl result;
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

void print_word_list(word_tbl wl) {
    char *is_upper,
         *is_twittertag;
    
    is_upper = (wl.is_capital ? "yes" : "no");
    is_twittertag = (wl.is_twittertag ? "yes" : "no");
    printf("Word: %-17s Capital: %-4s Twittertag: %s\n", wl.word, is_upper, is_twittertag);
}
