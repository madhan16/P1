/*Overall function*/
void find_hotspots_for_tweet (int removal_choice);

/*Functions for setting up the tagged tweet and getting it into structs*/
tagged_word_tbl *setup_tagged_tweet(int *number_of_words);
int count_words (FILE *ifp);
tagged_word_tbl *transfer_tagged_tweet_to_structs(FILE *ifp, int number_of_words);
void load_tweet (FILE *ifp, tagged_word_tbl *tagged_tweet);
tagged_word_tbl split_word (char *temp);

/*Functions for finding the hotspots*/
int *find_adj_adv_hotspots(int number_of_words, tagged_word_tbl *tagged_tweet);
int check_for_same_tags(int *index, tagged_word_tbl *tagged_tweet, int number_of_words);

/*Functions for showing the hotspots
  NOT USED ANY MORE*/
void show_hotspots(int number_of_words, tagged_word_tbl *tagged_tweet, int count[]);
void print_hotspots(int curr_index, int curr_count, tagged_word_tbl *tweet);

/*Function for removing the hotspots*/
tagged_word_tbl *clean_hotspots(int *number_of_words, tagged_word_tbl *tagged_tweet, int count[]);
int determine_amount_words_to_remove(int curr_index, tagged_word_tbl *full_tweet,  int curr_count); 


void print_compressed_tweet_to_file(tagged_word_tbl *shorter_tweet, int amt_of_words);