/*Overall function*/
void find_hotspots_for_tweet (void);

/*Functions for setting up the tagged tweet and getting it into structs*/
tagged_word_tbl *setup_tagged_tweet(int *number_of_words);
int count_words (FILE *ifp);
tagged_word_tbl *transfer_tagged_tweet_to_structs(FILE *ifp, int number_of_words);
void load_tweet (FILE *ifp, tagged_word_tbl *tagged_tweet);
tagged_word_tbl split_word (char *temp);

/*Functions for finding the hotspots*/
int *find_adj_adv_hotspots(int number_of_words, tagged_word_tbl *tagged_tweet);
int check_for_same_tags(int *index, tagged_word_tbl *tagged_tweet, int number_of_words);

/*Functions for showing the hotspots*/
void show_hotspots(int number_of_words, tagged_word_tbl *tagged_tweet, int count[]);
void print_hotspots(int curr_index, int curr_count, tagged_word_tbl *tweet);
