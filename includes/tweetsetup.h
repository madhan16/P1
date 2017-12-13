/*Main function for setting up the tweet*/
int split_to_words(char *str, twitter_words_tbl *wl, special_signs_tbl no_space_before, special_signs_tbl no_space_after, special_signs_tbl twitter_tags);

/*Function that checks if the input character (val) is in the array arr*/
int in_array(char val, special_signs_tbl); 

/*Function that returns an struct coantain the word and info about it*/
twitter_words_tbl transfer_word_to_list(twitter_words_tbl *wl, char *word);
/*Functions to get the info on words*/
int word_starts_upper(char *str);
int is_twitter_tag(char *str);

/*Function to make the words lower case because our databases are lower case
  and then upper case agian later*/
void make_all_words_lowercase(twitter_words_tbl *wl, int n_words);
void start_with_capital_agian(twitter_words_tbl *wl, int n_words);

/*Functions for printing a tweet to a file*/
void print_tweet_to_file(twitter_words_tbl *wlc, int n_words, char write_path[], special_signs_tbl no_space_before, special_signs_tbl no_space_after);
int add_space_or_not(char *curr_word, char *last_word, special_signs_tbl no_space_before, special_signs_tbl no_space_after);

/*Function for if we want to print one of the words from the tweet*/
void print_word_list(twitter_words_tbl wl);
