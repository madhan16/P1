/*PROTOTYPES FOR TWEET SETUP*/
int in_array(char val, char *arr);
int split_to_words(char *str, twitter_words_tbl *wl);
twitter_words_tbl transfer_word_to_list(twitter_words_tbl *wl, char *word);
void print_word_list(twitter_words_tbl wl);
int word_starts_upper(char *str);
int is_twitter_tag(char *str);
void make_all_words_lowercase(twitter_words_tbl *wl, int n_words);