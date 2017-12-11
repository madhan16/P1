/*Overall function*/
void find_synonyms_to_tweet(twitter_words_tbl *wl, int words_in_tweet);

/*Functions for doing a binary search*/
long find_word_in_file(FILE *ifp, char *search_for, unsigned long *out_longest_line);
unsigned long *index_lines_in_file(FILE *ifp, unsigned long *out_number_of_lines, unsigned long *out_longest_line);
void read_word_in_line(FILE *ifp, unsigned long line_start_byte, unsigned long longest_line, char *out_word);
long binary_search(FILE *ifp, unsigned long *line_positions, unsigned long longest_line, 
                   unsigned long left, unsigned long right, char *search_for);

/*Function for getting the synonyms when on the right file line*/
int get_synonyms_from_fileline(FILE *ifp, int byte_index, int max_line_size, synonym_tbl *shorter_synonyms);

/*Function for selection only the shorter synonyms*/
int find_synonyms_shorter_than_word(char synonyms[][MAX_WORD_LGT], synonym_tbl *ss, int amt_synonyms,
                                   char *cur_word);
/*Function for printing the shorter synonyms*/
void print_shorter_synonyms(synonym_tbl *shorter_synonyms, int amt_shorter_synonyms);
