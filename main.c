#include "includes/StructsAndConsts.h"
#include "includes/tweetsetup.h"
#include "includes/abbreviate.h"
#include "includes/synonyms.h"
#include "includes/hotspots.h"

/*Our compress tweet function that calls all the other functions*/
void compress_tweet(twitter_words_tbl *wl, int wl_words, abb_tbl *abb_list, int abb_lines);

int main(void) {
    int count_twitter_words, abb_tbl_lines;
    char tweet[MAX_AMT_SIGNS_IN_TWEET];
    FILE *ifp;
    abb_tbl *abb_list;
    twitter_words_tbl word_list[MAX_AMT_WORDS_IN_TWEET];

    ifp = fopen(TWEET_PATH, "r");
    fgets(tweet, MAX_AMT_SIGNS_IN_TWEET, ifp);
    fclose(ifp);
        
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
    int amt_words_abb_tweet;
    twitter_words_tbl compressed_tweet[MAX_AMT_WORDS_IN_TWEET];
    
    /*Add abbreviations to tweet*/
    amt_words_abb_tweet = add_abbreviation_to_tweet(wl, wl_words, abb_list, abb_lines, compressed_tweet);
    
    /*Find synonyms to tweet*/
    find_synonyms_to_tweet(compressed_tweet, wl_words);
    
    /*Here we print the abbreviated tweet to a txt file, because Twitie-tagger needs
      the tweet on txt-form to tag it*/
    print_tweet_to_file(compressed_tweet,  amt_words_abb_tweet); /*abbreviated_tweet.txt*/

    /*Find multiple adjectives or/and adverbs*/
    find_hotspots_for_tweet();
}
