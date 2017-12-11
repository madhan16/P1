#include "../includes/StructsAndConsts.h"
#include "../includes/synonyms.h"

/*FIND SYNONYMS FOR TWEET*/
/*STREAMS SYNONYMS FROM FILE AND MATCHES WITH TWEET*/
void find_synonyms_to_tweet(twitter_words_tbl *wl, int words_in_tweet)
{
	synonym_tbl shorter_synonyms[AMT_SYN];
	int i, amt_shorter_synonyms;
	long start_byte;
	unsigned long *synonym_db_line_indecies;
	long synonym_db_number_of_lines;
	long synonym_db_longest_line;

	FILE *ifp = fopen(SYNONYM_PATH, "r");

	if (ifp == NULL)
	{
		printf("Could not connect to synonyms file\n");
		exit(EXIT_FAILURE);
	}

	synonym_db_line_indecies = index_lines_in_file(ifp, &synonym_db_number_of_lines, &synonym_db_longest_line);

	/*THERE SEEMS TO BE SOME PROBLEM WITH COMMAS AND DOTS*/
	for (i = 0; i < words_in_tweet; i++)
	{
		start_byte = find_word_in_file(ifp, wl[i].word, synonym_db_line_indecies, synonym_db_number_of_lines, synonym_db_longest_line);
		//printf("Word: %s Start_byte: %d\n", wl[i].word, start_byte);
		if (start_byte > -1)
		{
			amt_shorter_synonyms = get_synonyms_from_fileline(ifp, start_byte, synonym_db_longest_line, shorter_synonyms);
			// printf("Amount of shorter synonyms: %d\n", amt_shorter_synonyms);
			print_shorter_synonyms(shorter_synonyms, amt_shorter_synonyms);
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
        // printf("Found \"%s\" in line %d\n", search_for, line_number);
        start_byte = synonym_db_line_indecies[line_number];
    }

    else
    {
        // printf("Did not find \"%s\"\n", search_for);
    }

    return start_byte;
}

unsigned long *index_lines_in_file(FILE *ifp, long *out_number_of_lines, long *out_longest_line)
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

void read_word_in_line(FILE *ifp, unsigned long line_start_byte, long longest_line, char *out_word) {
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
int get_synonyms_from_fileline(FILE *ifp, int byte_index, int max_line_size, synonym_tbl *shorter_synonyms) {
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
			/*move filepointer one backwards so we can load the first
			  char into our word*/
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