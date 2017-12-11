#include "../includes/StructsAndConsts.h"
#include "../includes/tweetsetup.h"


/*This function splits the words in the tweet into indvidual structs*/
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

/*Checks if the chacter val is found in array*/
int in_array(char val, char *arr) {
    int i;
    int size = sizeof(arr);
    for(i = 0; i < size; i++) {
        if(arr[i] == val)
            return 1;
    }
    return 0;
}

/*Transfers word to word struct table*/
twitter_words_tbl transfer_word_to_list(twitter_words_tbl *wl, char *word) {
    twitter_words_tbl result;
    strcpy(result.word, word);
    result.is_capital = word_starts_upper(word);
    result.is_twittertag = is_twitter_tag(word);
    
    return result;
}

/*checks if string starts uppercase*/
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

/*takes an array of structs and replace first uppercase letter in word with lowercase*/
void make_all_words_lowercase(twitter_words_tbl *wl, int n_words) {
    int i;
    
    for(i = 0; i < n_words; i++) {
        if(wl[i].is_capital)
            wl[i].word[0] = tolower(wl[i].word[0]);
    }
}

/*can only print one element of array*/
void print_word_list(twitter_words_tbl wl) {
    char *is_upper,
         *is_twittertag;
    
    is_upper = (wl.is_capital ? "yes" : "no");
    is_twittertag = (wl.is_twittertag ? "yes" : "no");

    printf("Word: %-17s Capital: %-4s Twittertag: %s\n", wl.word, is_upper, is_twittertag);
}

/*prints abbreviated tweet to file for twitter tagger*/
void print_tweet_to_file(twitter_words_tbl *wlc, int n_words) {
    FILE *ofp = fopen(ABB_WRITE_PATH, "w");
    int i, space_or_not;
    
    for(i = 0; i < n_words; i++) {
        /*if not special signs add space after word*/
        space_or_not = add_space_or_not(wlc[i].word, wlc[i + 1].word);
        
        fprintf(ofp, "%s%s", wlc[i].word, space_or_not == 1 ? " " : "");
    }
    
    fclose(ofp);
}

/*Help function for printing to file*/
int add_space_or_not(char *str1, char *str2) {
    char signsWithSpaceAfter[10] = {'!', '?', '.', ',', ')'};
    
    if(in_array(*str2, signsWithSpaceAfter))
        return 0;
    
    return 1;
}