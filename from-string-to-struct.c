#include <stdio.h>
#include <ctype.h>
#include <string.h>
#define WORD_NAME_LGT 50
#define WORD_LIST_LGT 100

typedef struct word {
    char word[WORD_NAME_LGT];
    int is_capital;
} word_tbl;

int in_array(char val, char *arr);
int split_to_words(char *str, word_tbl *wl);
word_tbl transfer_word_to_list(word_tbl *wl, char *word);
void print_word_list(word_tbl wl);
int word_starts_upper(char *str);

int main(void) {
    int i, countWords;
    word_tbl word_list[WORD_LIST_LGT];
    char tweet[] = "Our boy @tobygreendj crushing it on @mintsyndicate rn! Big up @austinkramer";
    
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
    char specialSigns[10] = {'!', '?', '.', ',', '-', '_'};
    int countWords;
    
    for(i = 0; i <= (strlen(str)); i++) {
        if (isalnum(str[i])) {
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
        else if(in_array(str[i], specialSigns)) {
            /*complete temperary string found before special sign*/
            temp[j] = '\0';
            wl[k++] = transfer_word_to_list(wl, temp);
            j = 0;
            /*add special sign*/
            temp[j] = str[i]; temp[j + 1] = '\0';
            wl[k++] = transfer_word_to_list(wl, temp);
            j = 0;
            /*skip space after special sign*/
            i++;
        }
    }
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

void print_word_list(word_tbl wl) {
    char *yes_or_no;
    
    yes_or_no = (wl.is_capital ? "yes" : "no");
    printf("Word: %-17s Is capital: %s\n", wl.word, yes_or_no);
}
