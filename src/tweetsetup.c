#include "../includes/StructsAndConsts.h"
#include "../includes/tweetsetup.h"


/*This function splits the words in the tweet into indvidual structs*/
int split_to_words(char *str, twitter_words_tbl *wl, special_signs_tbl no_space_before, special_signs_tbl no_space_after) {
    int i, j = 0, k = 0;
    /*temperary string to store word*/
    char temp[30];
    /*special characters to check for*/
    int countWords;

    for(i = 0; i <= (strlen(str)); i++) {
        if(str[i] == ' ' || str[i] == '\0') {
            /*complete string*/
            temp[j] = '\0';
            wl[k++] = transfer_word_to_list(wl, temp);
            /*reset element*/
            j = 0;
        }
        /*if sign is in array*/
        else if(in_array(str[i], no_space_before)) {
            /*complete temperary string found before special sign*/
            temp[j] = '\0';
            wl[k++] = transfer_word_to_list(wl, temp);
            j = 0;
            /*add special sign*/
            temp[j++] = str[i];
        }
        else if(in_array(str[i], no_space_after)) {
            temp[j++] = str[i];
            temp[j] = '\0';
            wl[k++] = transfer_word_to_list(wl, temp);
            j = 0;
        }
        else {
            temp[j++] = str[i];
        }

    }
    
    temp[j] = '\0';
    wl[k++] = transfer_word_to_list(wl, temp);
    
    countWords = k;
    return countWords;
}

/*Checks if the chacter val is found in array*/
int in_array(char val, special_signs_tbl arr) {
    int i;
    for(i = 0; i < arr.size; i++) {
        if(arr.sign[i] == val) {
            //printf("%c", arr.sign[i]);
            return 1;
        }
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
    char *twitter_tags = {"#@$-\'\""};
    
    for(i = 0; i < 3; i++) {
        if(str[0] == twitter_tags[i])
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
/*Makse the words that have been transformed to lowercase upper-case agian*/
void start_with_capital_agian(twitter_words_tbl *wl, int n_words) {
    int i;
    for(i = 0; i < n_words; i++) {
        if(wl[i].is_capital) {
            wl[i].word[0] = toupper(wl[i].word[0]);
        }
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
void print_tweet_to_file(twitter_words_tbl *wlc, int n_words, char write_path[], special_signs_tbl no_space_before, special_signs_tbl no_space_after) {
    FILE *ofp = fopen(write_path, "w");
    int i, space_or_not = 0;
    
    start_with_capital_agian(wlc, n_words);

    for(i = 0; i < n_words; i++) {
        /*if not special signs add space after word*/
        if (i != 0)
            space_or_not = add_space_or_not(wlc[i].word, wlc[i-1].word, no_space_before, no_space_after);
        
        fprintf(ofp, "%s%s", space_or_not == 1 ? " " : "", wlc[i].word);
    }
    
    fclose(ofp);
}

/*Help function for printing to file*/
int add_space_or_not(char *curr_word, char *last_word, special_signs_tbl no_space_before, special_signs_tbl no_space_after) {
    if(in_array(*curr_word, no_space_before))
        return 0;
    else if(in_array(*last_word, no_space_after))
        return 0;
    
    return 1;
}