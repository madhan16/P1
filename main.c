#include "includes/StructsAndConsts.h"
#include "includes/tweetsetup.h"
#include "includes/abbreviate.h"
#include "includes/synonyms.h"
#include "includes/hotspots.h"

enum functions{abbr = 1, syns, hotspots, all};
/*Our compress tweet function that calls all the other functions*/
void compress_tweet(twitter_words_tbl *full_tweet, int amt_words_full_tweet, abb_tbl *abb_list, int abb_lines, special_signs_tbl no_space_before, special_signs_tbl no_space_after);
void user_interaction(int *function_choice, int *remove_or_not);

int main(void) {
    int count_twitter_words, abb_tbl_lines, full_tweet_len;
    char full_tweet_str[MAX_AMT_SIGNS_IN_TWEET];
    char temp_str[MAX_AMT_SIGNS_IN_TWEET];

    special_signs_tbl no_space_before = {")!?.,", 5},
                      no_space_after = {"(\n", 2},
                      twitter_tags = {"#@$-\'", 5};

    printf("%s\n", no_space_after.sign);
    FILE *ifp;
    abb_tbl *abb_list;
    twitter_words_tbl word_list[MAX_AMT_WORDS_IN_TWEET];

    ifp = fopen(TWEET_PATH, "r");
    fgets(temp_str, MAX_AMT_SIGNS_IN_TWEET, ifp);
    strcpy(full_tweet_str, temp_str);
    
    while (fgets(temp_str, MAX_AMT_SIGNS_IN_TWEET, ifp)) {
        full_tweet_len = strlen(full_tweet_str);            
        strcpy(full_tweet_str + full_tweet_len, temp_str);
    }
    
    fclose(ifp);
    /*Setting up the tweet in structs*/
    count_twitter_words = split_to_words(full_tweet_str, word_list, no_space_before, no_space_after, twitter_tags);
    make_all_words_lowercase(word_list, count_twitter_words);
    
    /*Setting up (loading) the abbreviation database*/
    abb_list = abbreviation_setup(&abb_tbl_lines);

    /*Compressing the tweet*/
    compress_tweet(word_list, count_twitter_words, abb_list, abb_tbl_lines, no_space_before, no_space_after);
    
    /*Freeing the allocated memory agian*/
    free(abb_list);
    return 0;
}

/*COMPRESS TWEET*/
void compress_tweet(twitter_words_tbl *full_tweet, int amt_words_full_tweet, abb_tbl *abb_list, int abb_lines, special_signs_tbl no_space_before, special_signs_tbl no_space_after) {
    int amt_words_abb_tweet, func_choice = 0, removal_choice = 0;
    twitter_words_tbl compressed_tweet[MAX_AMT_WORDS_IN_TWEET];
    
    user_interaction(&func_choice, &removal_choice);
    
    switch (func_choice) {
        case abbr:
            /*We check our abbrevation data-base, and see if we can abberviate any of the words*/
            amt_words_abb_tweet = add_abbreviation_to_tweet(full_tweet, amt_words_full_tweet, abb_list, abb_lines, compressed_tweet);
            
            /*Then we write the shorter tweet onto a new txt file*/
            print_tweet_to_file(compressed_tweet, amt_words_abb_tweet, COMPRESSED_WRITE_PATH, no_space_before, no_space_after); /*../compressed.txt*/        
            break;

        case syns:
            /*We search trough our tweet and display the cases where we know shorter synonyms*/
            find_synonyms_to_tweet(full_tweet, amt_words_full_tweet);
            
            /*We do this just in case there is an old compressed file*/
            print_tweet_to_file(full_tweet, amt_words_full_tweet, COMPRESSED_WRITE_PATH, no_space_before, no_space_after);
            break;

        case hotspots: 
            /*First we need the tweet to be on txt.form agian for the twitie-tagger to work*/
            print_tweet_to_file(full_tweet, amt_words_full_tweet, UNTAGGED_WRITE_PATH, no_space_before, no_space_after); /*untagged_tweet.txt*/

            /*Then we can run our program that finds the hotspots and prints it to the file if removal was on*/
            find_hotspots_for_tweet(removal_choice);
            break;

        case all: /*In this case we do all of the above*/
            amt_words_abb_tweet = add_abbreviation_to_tweet(full_tweet, amt_words_full_tweet, abb_list, abb_lines, compressed_tweet);
    
            print_tweet_to_file(compressed_tweet, amt_words_abb_tweet, UNTAGGED_WRITE_PATH, no_space_before, no_space_after); /*untagged_tweet.txt*/
            
            /*We do this because if we dont remove the adverbs, adjectives, then the text wont get any shorter from here*/
            if (removal_choice == 2) 
                print_tweet_to_file(compressed_tweet, amt_words_abb_tweet, COMPRESSED_WRITE_PATH, no_space_before, no_space_after); /*../compressed.txt*/
            
            find_synonyms_to_tweet(compressed_tweet, amt_words_abb_tweet);

            find_hotspots_for_tweet(removal_choice);
            break;

        default: printf("Something went wrong\n!"); break;
    }
    printf("\nDone!\n");
}

void user_interaction(int *function_choice, int *remove_or_not) {
    int done = 0;
   
    printf("Which functions do you want to run:\n"
           "(1) Abbrevation\n"
           "(2) Find shorter synonyms\n"
           "(3) Find many adjectives and adverbs\n"
           "(4) All of the above\n");
     do {
        scanf("%d", function_choice);
        if (*function_choice > 0 && *function_choice < 5)
            done = 1; 
        else 
            printf("Invalid input! Please enter another input.\n");       
    } while (!done);
    
    if (*function_choice == 3 || *function_choice == 4) {
         printf("Do you want to remove the extra adverbs and adjectives (BETA)\n"
           "(1) Yes\n"
           "(2) NO\n");
        scanf("%d", remove_or_not);
    }
}
