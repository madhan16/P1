/*Abbreviation database setup*/ 
abb_tbl *abbreviation_setup(int *lines);
int count_lines_in_file(FILE *abbfp);
void fill_array_from_file(FILE *abbfp, abb_tbl *arr, int *lines);
abb_tbl read_file_line(FILE *abbfp);
int count_words_in_string(char *str);

/*Doing the Abbreviation*/ 
int add_abbreviation_to_tweet(twitter_words_tbl *wl, int wl_words, abb_tbl *abb_list, int abb_lines, twitter_words_tbl *wlc);
int analyse_word(twitter_words_tbl *wl, int wl_words, int *word_index, abb_tbl *abb_list, int abb_words);
int check_and_combine_multiple_words (int n_abb_words, int word_index, int amt_words_tweet, twitter_words_tbl *wl, char *temp_word);
int abb_is_multiple_words(int n_abb_words, int word_index, int amt_words_tweet);
