#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE 200
#define MAX_WORD_LGT 50
#define WORD_LIST_LGT 100
#define AMT_SYN 100

/*change to path /WIN/ if windows*/
/*change to path /MAC/ if mac*/
#define ABB_PATH "files/WIN/shortened.txt"
#define ABB_WRITE_PATH "files/WIN/abbreviated_tweet.txt"
#define SYNONYM_PATH "files/WIN/synonymdatabase.txt"

typedef struct word
{
    char word[MAX_WORD_LGT];
    int is_capital;
    int is_twittertag;
} twitter_words_tbl;

typedef struct abb
{
    char word[MAX_WORD_LGT];
    char abb[MAX_WORD_LGT];
    int n_words;
} abb_tbl;

typedef struct synonyms
{
    char word[MAX_WORD_LGT];
    char synonym[MAX_WORD_LGT];
} synonym_tbl;

long synonym_db_number_of_lines;
long synonym_db_longest_line;

int in_array(char val, char *arr);
int split_to_words(char *str, twitter_words_tbl *wl);
twitter_words_tbl transfer_word_to_list(twitter_words_tbl *wl, char *word);
void print_word_list(twitter_words_tbl wl);
int word_starts_upper(char *str);
int is_twitter_tag(char *str);

/*abbreviate functions*/
abb_tbl *abbreviation_setup(int *lines);
int count_lines_in_file(FILE *abbfp);
void fill_array_from_file(FILE *abbfp, abb_tbl *arr, int *lines);
abb_tbl read_file_line(FILE *abbfp);
int count_words_in_string(char *str);
void print_abb_list(abb_tbl *arr, int *lines);
void make_all_words_lowercase(twitter_words_tbl *wl, int n_words);

/*compress tweet*/
void compress_tweet(twitter_words_tbl *wl, int wl_words, abb_tbl *abb_list, int abb_lines, unsigned long *synonym_db_line_indecies);

/*abbreviate tweet*/
int add_abbreviation_to_tweet(twitter_words_tbl *wl, int wl_words, abb_tbl *abb_list,
                              int abb_lines, twitter_words_tbl *wlc);
int analyse_word(twitter_words_tbl *wl, int wl_words, int *word_index,
                 abb_tbl *abb_list, int abb_words);
int abb_is_multiple_words(abb_tbl *abb_list, int index, int word_index, int wl_words);
void print_abbreviated_tweet_to_file(twitter_words_tbl *wlc, int n_words);
int add_space_or_not(char *str1, char *str2);

/*find synonyms*/
void find_synonyms_to_tweet(twitter_words_tbl *wl, int words_in_tweet, unsigned long *synonym_db_line_indecies);
int get_synonyms_from_fileline(FILE *ifp, int byte_index, int max_line_size, synonym_tbl *shorter_synonyms);
long binary_search(FILE *ifp, unsigned long *synonym_db_line_indecies, long longest_line, long left, long right, char *search_for);
long find_word_in_file(FILE *ifp, char *search_for, unsigned long *synonym_db_line_indecies, long number_of_lines, long longest_line);
void read_word_in_line(FILE *ifp, unsigned long line_start_byte, unsigned long longest_line, char *out_word);
unsigned long *index_lines_in_file(FILE *ifp, unsigned long *out_number_of_lines, unsigned long *out_longest_line);
int find_synonyms_shorter_than_word(char synonyms[][MAX_WORD_LGT], synonym_tbl *ss, int amt_synonyms,
                                    char *cur_word);
void print_shorter_synonyms(synonym_tbl *shorter_synonyms, int amt_shorter_synonyms);

int main(void)
{
    int count_twitter_words, abb_tbl_lines;
    abb_tbl *abb_list;
    twitter_words_tbl word_list[WORD_LIST_LGT];

    char tweet[] = "Depression night in September, hello damper as soon as possible. Is not as dampness to we are in this laughing out loud daily Alaska when Professor Doctor family died as expected! dampness?!?!?!?!?";

    /*abbreviation setup*/
    abb_list = abbreviation_setup(&abb_tbl_lines);

    /*setup tweet*/
    count_twitter_words = split_to_words(tweet, word_list);
    make_all_words_lowercase(word_list, count_twitter_words);

    /* Set up synonym database */
    unsigned long *synonym_db_line_indecies;
    FILE *ifp = fopen(SYNONYM_PATH, "r");
    synonym_db_line_indecies = index_lines_in_file(ifp, &synonym_db_number_of_lines, &synonym_db_longest_line);
    find_word_in_file(ifp, "doctor", synonym_db_line_indecies, synonym_db_number_of_lines, synonym_db_number_of_lines);
    fclose(ifp);
    /*compress tweet*/
    // compress_tweet(word_list, count_twitter_words, abb_list, abb_tbl_lines, synonym_db_line_indecies);

    free(abb_list);
    free(synonym_db_line_indecies);
    return 0;
}

/*TWEET FUNCTIONS*/
/*TAKES A TWEET STRING FROM MAIN AND SPLITS INTO WORDS IN ARRAY OF
  STRUCTS CALLED TWITTER_WORDS_TBL*/

int split_to_words(char *str, twitter_words_tbl *wl)
{
    int i, j = 0, k = 0;
    /*temperary string to store word*/
    char temp[30];
    /*special characters to check for*/
    char signsWithSpaceAfter[10] = {'!', '?', '.', ',', ')'};
    char specialSigns[10] = {'('};
    char twitterTags[10] = {'#', '@', '$', '-', '\''};
    int countWords;

    for (i = 0; i <= (strlen(str)); i++)
    {
        if (isalnum(str[i]) || in_array(str[i], twitterTags))
        {
            temp[j++] = str[i];
        }
        else if (str[i] == ' ' || str[i] == '\0')
        {
            /*complete string*/
            temp[j] = '\0';
            wl[k++] = transfer_word_to_list(wl, temp);
            /*reset element*/
            j = 0;
        }
        /*if sign is in array*/
        else if (in_array(str[i], signsWithSpaceAfter))
        {
            /*complete temperary string found before special sign*/
            temp[j] = '\0';
            wl[k++] = transfer_word_to_list(wl, temp);
            j = 0;
            /*add special sign*/
            temp[j++] = str[i];
        }
        else if (in_array(str[i], specialSigns))
        {
            temp[j++] = str[i];
            temp[j] = '\0';
            wl[k++] = transfer_word_to_list(wl, temp);
            j = 0;
        }
    }

    temp[j] = '\0';
    wl[k++] = transfer_word_to_list(wl, temp);

    countWords = k;
    return countWords;
}

/*checks if val is found in array*/
int in_array(char val, char *arr)
{
    int i;
    int size = sizeof(arr);
    for (i = 0; i < size; i++)
    {
        if (arr[i] == val)
            return 1;
    }
    return 0;
}

/*transfers word to word table*/
twitter_words_tbl transfer_word_to_list(twitter_words_tbl *wl, char *word)
{
    twitter_words_tbl result;
    strcpy(result.word, word);
    result.is_capital = word_starts_upper(word);
    result.is_twittertag = is_twitter_tag(word);

    return result;
}

/*checks wheter string starts uppercase*/
int word_starts_upper(char *str)
{
    char ch = str[0];
    if (ch >= 'A' && ch <= 'Z')
        return 1;
    else
        return 0;
}

int is_twitter_tag(char *str)
{
    int i;
    char twitterTags[10] = {'#', '@', '$'};

    for (i = 0; i < 3; i++)
    {
        if (twitterTags[i] == str[0])
            return 1;
    }
    return 0;
}

/*can only print one element of array*/
void print_word_list(twitter_words_tbl wl)
{
    char *is_upper,
        *is_twittertag;

    is_upper = (wl.is_capital ? "yes" : "no");
    is_twittertag = (wl.is_twittertag ? "yes" : "no");

    printf("Word: %-17s Capital: %-4s Twittertag: %s\n", wl.word, is_upper, is_twittertag);
}

/*takes an array of structs and replace first uppercase letter in word with lowercase*/
void make_all_words_lowercase(twitter_words_tbl *wl, int n_words)
{
    int i;

    for (i = 0; i < n_words; i++)
    {
        if (wl[i].is_capital)
            wl[i].word[0] = tolower(wl[i].word[0]);
    }
}

/*ABBREVIATION FUNCTIONS*/
/*READS TXT FILE WITH ABBREVIATION WORDS AND PUTS INTO ARRAY OF STRUCTS
  CALLED ABB_TBL*/

abb_tbl *abbreviation_setup(int *lines)
{
    abb_tbl *abb_list;
    FILE *ifp = fopen(ABB_PATH, "r");
    /*check if connection was successfull*/
    if (ifp == NULL)
    {
        printf("Could not connect to file\n");
        exit(EXIT_FAILURE);
    }
    *lines = count_lines_in_file(ifp);
    abb_list = (abb_tbl *)malloc(*lines * sizeof(abb_tbl));

    fill_array_from_file(ifp, abb_list, lines);
    fclose(ifp);

    return abb_list;
}

/*takes a file points and returns number of lines in file*/
int count_lines_in_file(FILE *abbfp)
{
    int lines = 0;
    char temp_str[MAX_LINE];

    while (fgets(temp_str, MAX_LINE, abbfp))
    {
        lines++;
    }
    rewind(abbfp);

    return lines;
}

/*takes a file pointer, array of structs and size of array and fill 
  array of struct from file*/
void fill_array_from_file(FILE *abbfp, abb_tbl *arr, int *lines)
{
    int i;
    for (i = 0; i < *lines; i++)
        arr[i] = read_file_line(abbfp);
}

abb_tbl read_file_line(FILE *abbfp)
{
    abb_tbl p;

    fscanf(abbfp, " %[^:] : %[^\n] ", p.word, p.abb);
    p.n_words = count_words_in_string(p.word);

    return p;
}

/*returns number of words in string*/
int count_words_in_string(char *str)
{
    int i, count = 0;

    for (i = 0; i < strlen(str); i++)
    {
        if (str[i] == ' ')
            count++;
    }
    return count + 1;
}

/*prints abbreviation list*/
void print_abb_list(abb_tbl *arr, int *lines)
{
    int i;

    printf("%-4s %-30s %-10s %2s\n", "#", "WORD", "ABB.", "N");

    for (i = 0; i < *lines; i++)
        printf("%-4d %-30s %-10s %2d\n", (i + 1), arr[i].word, arr[i].abb, arr[i].n_words);
}

/*COMPRESS TWEET*/
/*MAKES A TWEET MORE READABLE*/

void compress_tweet(twitter_words_tbl *wl, int wl_words, abb_tbl *abb_list, int abb_lines, unsigned long *synonym_db_line_indecies)
{
    int n_words_abb_list;
    twitter_words_tbl compressed_tweet[WORD_LIST_LGT];

    /*add abbreviations to tweet*/
    n_words_abb_list = add_abbreviation_to_tweet(wl, wl_words, abb_list, abb_lines, compressed_tweet);
    print_abbreviated_tweet_to_file(compressed_tweet, n_words_abb_list); /*abbreviated_tweet.txt*/

    /*find synonyms to tweet*/
    find_synonyms_to_tweet(wl, wl_words, synonym_db_line_indecies);

    /*find multiple adjectives or/and adverbs*/
}

/*returns abbreviated tweet via pointer to array of structs and returns amount of words added*/
int add_abbreviation_to_tweet(twitter_words_tbl *wl, int wl_words, abb_tbl *abb_list,
                              int abb_words, twitter_words_tbl *wlc)
{
    int i, k = 0;
    int result_id = 0;
    for (i = 0; i < wl_words; i++)
    {
        result_id = analyse_word(wl, wl_words, &i, abb_list, abb_words);

        if (result_id == -1)
            wlc[k++] = transfer_word_to_list(wlc, wl[i].word);
        else
            wlc[k++] = transfer_word_to_list(wlc, abb_list[result_id].abb);
    }
    /*where k is amount of words added*/
    return k;
}

int analyse_word(twitter_words_tbl *wl, int wl_words, int *word_index,
                 abb_tbl *abb_list, int abb_words)
{
    int i, j, str_len;
    char curr_word[MAX_WORD_LGT];
    char temp_word[MAX_WORD_LGT * 3];
    strcpy(curr_word, wl[*word_index].word);

    for (i = 0; i < abb_words; i++)
    {
        strcpy(temp_word, curr_word);
        j = 1;
        if (abb_is_multiple_words(abb_list, i, *word_index, wl_words))
        {
            for (j = 1; j < abb_list[i].n_words; j++)
            {
                str_len = strlen(temp_word);
                strcpy(temp_word + str_len, " ");
                strcpy(temp_word + str_len + 1, wl[*word_index + j].word);
            }
        }
        if ((strcmp(temp_word, abb_list[i].word) == 0))
        {
            *word_index += (j - 1);
            return i;
        }
    }
    return -1;
}

/*returns 1 if strings contains more than one word and stops from
  reading through array elements*/
int abb_is_multiple_words(abb_tbl *abb_list, int index, int word_index, int wl_words)
{
    return abb_list[index].n_words > 1 &&
           abb_list[index].n_words + word_index < wl_words;
}

/*prints abbreviated tweet to file for twitter tagger*/
void print_abbreviated_tweet_to_file(twitter_words_tbl *wlc, int n_words)
{
    FILE *ofp = fopen(ABB_WRITE_PATH, "w");
    int i, space_or_not;

    for (i = 0; i < n_words; i++)
    {
        /*if not special signs add space after word*/
        space_or_not = add_space_or_not(wlc[i].word, wlc[i + 1].word);

        fprintf(ofp, "%s%s", wlc[i].word, space_or_not == 1 ? " " : "");
    }

    fclose(ofp);
}

int add_space_or_not(char *str1, char *str2)
{
    char signsWithSpaceAfter[10] = {'!', '?', '.', ',', ')'};

    if (in_array(*str2, signsWithSpaceAfter))
        return 0;

    return 1;
}

/*FIND SYNONYMS FOR TWEET*/
/*STREAMS SYNONYMS FROM FILE AND MATCHES WITH TWEET*/

void find_synonyms_to_tweet(twitter_words_tbl *wl, int words_in_tweet, unsigned long *synonym_db_line_indecies)
{
    synonym_tbl shorter_synonyms[AMT_SYN];
    int i, amt_shorter_synonyms;
    unsigned long longest_line;
    long start_byte = -1;

    FILE *ifp = fopen(SYNONYM_PATH, "r");

    if (ifp == NULL)
    {
        printf("Could not connect to file\n");
        exit(EXIT_FAILURE);
    }
    /*THERE SEEMS TO BE SOME PROBLEM WITH COMMAS AND DOTS*/
    for (i = 0; i < words_in_tweet; i++)
    {
        printf("Check for synonym: %s", wl[i].word);
        start_byte = find_word_in_file(ifp, wl[i].word, synonym_db_line_indecies, synonym_db_number_of_lines, synonym_db_longest_line);
        //printf("Word: %s Start_byte: %d\n", wl[i].word, start_byte);
        if (start_byte > -1)
        {
            printf(" - HAS synonym\n");
            // printf("found a word in synonym db\n");
            // amt_shorter_synonyms = get_synonyms_from_fileline(ifp, start_byte, synonym_db_longest_line, shorter_synonyms);
            // print_shorter_synonyms(shorter_synonyms, amt_shorter_synonyms);
        }
        else
        {
            printf(" - no synonym \n");
        }
    }
    fclose(ifp);
}

/* ======================================================
    Search function for finding word in synonym database
   ======================================================*/
long find_word_in_file(FILE *ifp, char *search_for, unsigned long *synonym_db_line_indecies, long number_of_lines, long longest_line)
{
    fseek(ifp, 0, SEEK_SET);
    long start_byte;

    long line_number = binary_search(ifp, synonym_db_line_indecies, longest_line, 0, number_of_lines - 1, search_for);

    start_byte = -1;

    if (line_number > -1)
    {
        printf("Found \"%s\" in line %d\n", search_for, line_number);
        start_byte = synonym_db_line_indecies[line_number];
    }

    else
    {
        printf("Did not find \"%s\"\n", search_for);
    }

    return start_byte;
}

unsigned long *index_lines_in_file(FILE *ifp, unsigned long *out_number_of_lines, unsigned long *out_longest_line)
{
    size_t position_array_size = 100;
    unsigned long *positions = malloc(position_array_size * sizeof(*positions));

    unsigned long i = 0;
    char *buff = malloc(1000 * sizeof(*buff));

    unsigned long longest_line = 0;

    positions[i++] = ftell(ifp);
    while (fgets(buff, 1000, ifp) != NULL)
    {
        if (strlen(buff) > longest_line)
        {
            longest_line = strlen(buff);
        }

        if (i >= position_array_size)
        {
            position_array_size += 100;
            unsigned long *realloced_array = realloc(positions, position_array_size * sizeof(*realloced_array));
            if (realloced_array)
            {
                positions = realloced_array;
            }
            else
            {
                printf("Error reallocating memory\n");
                exit(EXIT_FAILURE);
            }
        }
        positions[i++] = ftell(ifp);
    }

    free(buff);

    *out_number_of_lines = i - 1;
    *out_longest_line = longest_line;
    return positions;
}

void read_word_in_line(FILE *ifp, unsigned long line_start_byte, unsigned long longest_line, char *out_word) {
    char buffer[longest_line];
    int buffer_length = sizeof(buffer) / sizeof(buffer[0]);

    fseek(ifp, line_start_byte, SEEK_SET);

    fgets(buffer, buffer_length, ifp);

    char *token = strtok(buffer, "\t");
    if (token == NULL)
    {
        strcpy(out_word, "");
    }
    else
    {
        strcpy(out_word, token);
    }
}

long binary_search(FILE *ifp, unsigned long *line_positions, long longest_line, long left, long right, char *search_for) {
    while (left <= right) {
        long mid = left + (right - left) / 2;

        char word_in_file[longest_line];
        read_word_in_line(ifp, line_positions[mid], longest_line, word_in_file);
        int cmp = strcmp(search_for, word_in_file);

        /* Check if we have found the correct word */
        if (cmp == 0)
        {
            return mid;
        }
        /* Ignore left half */
        if (cmp > 0)
        {
            left = mid + 1;
        }
        /* Ignore right half */
        else
        {
            right = mid - 1;
        }

        /* Allows mid to be 0 one time, but not again */
        if (mid < 1)
        {
            break;
        }
    }
    /* Result not found */
    return -1;
}

/* ======================================================
    END OF SEARCH FUNCTION
   ======================================================*/

/*takes a file pointer to byteindex and finds shorter synonyms in file*/
int get_synonyms_from_fileline(FILE *ifp, int byte_index, int max_line_size, synonym_tbl *shorter_synonyms)
{
    int done = 0, i = 0, amt_shorter_synonyms, first_synonym_len;
    char synonyms[AMT_SYN][MAX_WORD_LGT];
    char temp_char, cur_word[20];
    /*move file pointer to the correct line (from search)*/
    fseek(ifp, byte_index, SEEK_SET);
    /*skip first word in file*/
    fscanf(ifp, " %s ", cur_word);

    while (!done)
    {
        /*get first synonym and skip [x]:*/
        fscanf(ifp, " %[^[] %*[^:] : ", synonyms[i]);
        /*Unfortuanlty we get the space before the [ into the word here, so we remove that:*/
        first_synonym_len = strlen(synonyms[i]);
        synonyms[i][first_synonym_len - 1] = '\0';
        i++;
        do
        {
            /*get synonyms seperated by ; or \t*/
            fscanf(ifp, " %[^;\t]", synonyms[i++]);
            temp_char = getc(ifp);

        } while (!(temp_char == '\t' || temp_char == EOF || temp_char == '\n'));

        while (temp_char == '\t')
            temp_char = getc(ifp);

        /*done conditions*/
        if (temp_char == '\n' || temp_char == EOF)
            done = 1;
        else
            /*move filepointer one backwards*/
            fseek(ifp, -1, SEEK_CUR);
    }
    amt_shorter_synonyms = find_synonyms_shorter_than_word(synonyms, shorter_synonyms, i, cur_word);

    /*printf("Curr word: %s\n", cur_word);
    printf("Synonyms:\n");
    for(i = 0; i < shorter_synonyms_words; i++)
        printf("%s\n", shorter_synonyms[i].synonym);*/
    return amt_shorter_synonyms;
}

/*finds synonyms in array which are shorter than curr_word*/
int find_synonyms_shorter_than_word(char synonyms[][MAX_WORD_LGT], synonym_tbl *ss, int amt_synonyms,
                                    char *cur_word)
{
    int i, j = 0, len_cur_word, len_synonym;
    double percent_reduction;

    len_cur_word = strlen(cur_word);

    for (i = 0; i < amt_synonyms; i++)
    {
        len_synonym = strlen(synonyms[i]);
        if ((strlen(synonyms[i])) < len_cur_word)
        {
            percent_reduction = (double)(len_cur_word - len_synonym) / len_cur_word * 100;
            if (percent_reduction >= 25)
            {
                strcpy(ss[j].word, cur_word);
                strcpy(ss[j].synonym, synonyms[i]);
                j++;
            }
        }
    }
    /*number of synonyms added*/
    return j;
}

void print_shorter_synonyms(synonym_tbl *shorter_synonyms, int amt_shorter_synonyms)
{
    int i;
    printf("Suggested synonyms for: %s\n", shorter_synonyms[0].word);
    for (i = 0; i < amt_shorter_synonyms; i++)
    {
        printf("%d: %s\n", i + 1, shorter_synonyms[i].synonym);
    }
}
