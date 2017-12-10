#include "includes/StructsAndConsts.h"
#include "includes/tweetsetup.h"
#include "includes/abbreviate.h"
#include "includes/synonyms.h"

/*Our compress tweet function that calls all the other functions*/
void compress_tweet(twitter_words_tbl *wl, int wl_words, abb_tbl *abb_list, int abb_lines);

int main(void) {
    int count_twitter_words, abb_tbl_lines;
    abb_tbl *abb_list;
    twitter_words_tbl word_list[MAX_AMT_WORDS_IN_TWEET];
    
    char tweet[] = "Depression night in September, as soon as possible. Is not as going to we are in this laughing out loud daily Alaska when Professor Doctor family died as expected! Documents?!?!?!?!?";
    
    /*Setting up the tweet in structs*/
    count_twitter_words = split_to_words(tweet, word_list);
    make_all_words_lowercase(word_list, count_twitter_words);

    /*Setting up (loading) the abbreviation database*/
    abb_list = abbreviation_setup(&abb_tbl_lines);
    
    /*Compressing the tweet*/
    compress_tweet(word_list, count_twitter_words, abb_list, abb_tbl_lines);
    
    /*Freeing the allocated memory agian*/
    free(abb_list);
    return 0;
}

/*COMPRESS TWEET*/
void compress_tweet(twitter_words_tbl *wl, int wl_words, abb_tbl *abb_list, int abb_lines) {
    int n_words_abb_list;
    twitter_words_tbl compressed_tweet[MAX_AMT_WORDS_IN_TWEET];
    
    /*Add abbreviations to tweet*/
    n_words_abb_list = add_abbreviation_to_tweet(wl, wl_words, abb_list, abb_lines, compressed_tweet);
    
    /*Here we print the abbreviated tweet to a txt file, because Twitie-tagger needs
      the tweet on txt-form to tag it*/
    print_abbreviated_tweet_to_file(compressed_tweet, n_words_abb_list); /*abbreviated_tweet.txt*/
    
    /*Find synonyms to tweet*/
    find_synonyms_to_tweet(wl, wl_words);
    
    /*find multiple adjectives or/and adverbs*/
}
