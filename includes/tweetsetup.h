/*Main function for setting up the tweet*/
int tweet_setup(twitter_words_tbl *full_tweet, special_signs_tbl no_space_before, special_signs_tbl no_space_after);

/*Function for loading the tweet txt into a string*/
void make_tweet_str(char *full_tweet_str);


/*Functions used for splitting the str into induvidual structs*/
int split_to_words(char *str, twitter_words_tbl *wl, special_signs_tbl no_space_before, special_signs_tbl no_space_after);
int in_array(char val, special_signs_tbl); 

/*Function that returns an struct coantain the word and info about it*/
twitter_words_tbl transfer_word_to_list(twitter_words_tbl *wl, char *word);
int word_starts_upper(char *str);
int is_twitter_tag(char *str);

/*Function to make the words lower case because our databases are lower case
  and then upper case agian later*/
void make_all_words_lowercase(twitter_words_tbl *wl, int n_words);
void start_with_capital_agian(twitter_words_tbl *wl, int n_words);

/*Functions for printing a tweet to a file*/
void print_tweet_to_file(twitter_words_tbl *wlc, int n_words, char write_path[], special_signs_tbl no_space_before, special_signs_tbl no_space_after);
int add_space_or_not(char *curr_word, char *last_word, special_signs_tbl no_space_before, special_signs_tbl no_space_after);

