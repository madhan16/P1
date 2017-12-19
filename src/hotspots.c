#include "../includes/StructsAndConsts.h"
#include "../includes/hotspots.h"

/*int main(void) {
    find_hotspots_for_tweet();
    return 0;
}*/

void find_hotspots_for_tweet (int removal_choice) {
    int number_of_words, *count;
    tagged_word_tbl *tagged_tweet;
    tagged_word_tbl *shorter_tweet;
    int i;
    /*First we setup/load the tagged tweet*/
    tagged_tweet = setup_tagged_tweet(&number_of_words);

    /*Then we look for the hotspots*/
    count = find_adj_adv_hotspots(number_of_words, tagged_tweet);

    /*Then we either remove the hotspots*/
    if (removal_choice == 1) {
        shorter_tweet = clean_hotspots(&number_of_words, tagged_tweet, count);
        print_compressed_tweet_to_file(shorter_tweet, number_of_words);
    }
    /*Or we show the hotspots*/
    else { 
        show_hotspots(number_of_words, tagged_tweet, count);
    }

    /*Freeing memory again*/
    free(tagged_tweet);
    free(shorter_tweet);
    free(count);
}

/*================================================
    FUNCTIONS THAT SETUP/LOAD THE TAGGED TWEET
  ================================================*/
/*Overall function that calls the others, and the twitie-tagger before returning
  our struct with tagged words and tags*/
tagged_word_tbl *setup_tagged_tweet(int *number_of_words) {
    tagged_word_tbl *tagged_tweet;
    FILE *ifp;

    /*Here we first run the twitter-tagger maybe the path needs to be updated in the start of the call*/
    system("%JAVA_HOME%\\bin\\java -jar twitie_tag.jar models/gate-EN-twitter.model untagged_tweet.txt > tweet_tagged.txt");
    printf("\n");

    /*Then we open the now tagged tweet and count the number of words in it, this count is done diffrently than
      earlier because for an example , is not a word for it self but a part of the other word*/
    ifp = fopen(TAGGED_TWEET_PATH, "r");
    *number_of_words = count_words(ifp);

    /*Here we load our tweet on txt form over into an array of structs*/
    tagged_tweet =  transfer_tagged_tweet_to_structs(ifp, *number_of_words);

    fclose(ifp);
    return tagged_tweet;
}

/*Function for counting the amount of words in the tagged tweet*/
int count_words (FILE *ifp) {
  char temp_word[MAX_WORD_LGT];
  int i = 0;
  /*The 49 is MAX_WORD_LGT -1*/
  while (fscanf(ifp, "%49s", temp_word) == 1) { /*Is true while fscanf can retrieve a word*/
    i++;
  }

  rewind(ifp);
  return i; 
}

/*Function for alocating and filling the array of the structs with tagged words*/
tagged_word_tbl *transfer_tagged_tweet_to_structs(FILE *ifp, int number_of_words) {
    tagged_word_tbl *temp_struct_array;
    /*First we create the struct array*/
    temp_struct_array = (tagged_word_tbl*)malloc(number_of_words * sizeof(tagged_word_tbl));
    /*Then we start loading over the tweet data*/
    load_tweet(ifp, temp_struct_array);
    return temp_struct_array;
}

/*Function for loading the tagged words into the struct array*/
void load_tweet (FILE *ifp, tagged_word_tbl *tagged_tweet) {
    char temp_word[MAX_WORD_LGT];
    int index = 0;

    while(fscanf(ifp, "%49s", temp_word) == 1) {
        tagged_tweet[index] = split_word(temp_word);
        index++;
    }
}

/*Function that splits the word and tag*/
tagged_word_tbl split_word (char *temp) {
    tagged_word_tbl w;
    /*The number 2 is here because we are only interessted in JJ for now not JJs*/
    sscanf(temp," %[^_] _ %2s", w.word, w.tag);
    return w;
}

/*=========================
    END OF SETUP/LOADING
  =========================*/

/*====================================
    FUNCTIONS FOR FINDING HOTSPOTS
  ====================================*/

/*Function for finding the hot spots*/
int *find_adj_adv_hotspots(int number_of_words, tagged_word_tbl *tagged_tweet) {
    int *temp_array, i;
    /*We use calloc here instead to fill the counts with 0 at first*/
    temp_array = (int*)calloc(number_of_words, sizeof(int));

    for(i = 0; i < number_of_words; i++){
        temp_array[i] = check_for_key_tags(&i, tagged_tweet, number_of_words);
    }

    return temp_array;
}

/*Rekursive function that moves the index if it finds a adjectiv of adverb and 
  counts up a counter*/
int check_for_key_tags(int *index, tagged_word_tbl *tagged_tweet, int number_of_words) {
    /* JJ is the tag for adjectives
       RB is the tag for adverbs
    */
    int temp_count;
    static int count = 0;
    if(strcmp(tagged_tweet[*index].tag, "JJ") == 0) {
        *index += 1;
        /*We here make sure we dont look into memory after our struct*/
        if (*index < number_of_words) {
            temp_count = ++count;
            count = 0;
            return temp_count + check_for_key_tags(index, tagged_tweet, number_of_words);
        }            
        else {
            return count;
        }
           
    }
    else if (strcmp(tagged_tweet[*index].tag, "RB") == 0) {
        *index += 1;
        /*We here make sure we dont look into memory after our struct*/
        if (*index < number_of_words) {
            count++;
            return 0 + check_for_key_tags(index, tagged_tweet, number_of_words); 
        }
        else 
            return 0;
    }
    /*CC is word like and, or, and but that could connect more adjectives and adverbs but dont count to wards our counter*/
    else if (strcmp(tagged_tweet[*index].tag, "CC") == 0) {
        *index += 1;
        /*We here make sure we dont look into memory after our struct*/
        if (*index < number_of_words) 
            return 0 + check_for_key_tags(index, tagged_tweet, number_of_words); 
        else 
            return 0;
    }
    else {
        count = 0; /*Remeber to reset count*/
        return 0;
    }
}

/*=============================
    END OF FINDING HOTSPOTS
  =============================*/

/*=============================
    WORKING ON THE HOTSPOTS
  =============================*/

/*Need to rewrite this for in cases with no hotspots er noget med at tælle op også til sidst sige, hvis den ikke er over 1 
  så print istedet antallet af adjektiver/adverbs ialt. */
void show_hotspots(int number_of_words, tagged_word_tbl *tweet, int count[]) {
    int curr_index;
    printf("You have a lot of adjectives or adverbs in the following parts of your sentence, marked by numbers:\n");
    for (curr_index = 0; curr_index < number_of_words; curr_index++) {
        if (count[curr_index] >= HIGH_TAG_AMOUNT) {
            print_hotspots(curr_index, count[curr_index], tweet);
        }
    }
}

/*Function that prints one of the hotspots if there is found any*/
void print_hotspots(int start_index, int curr_count, tagged_word_tbl *tweet) {
    int i = 0, amt_conector_words = 0, curr_index;
        while (i < curr_count) {
            curr_index = start_index + i + amt_conector_words;
            if (strcmp(tweet[curr_index].tag, "CC") == 0) {
                printf("%s ", tweet[curr_index].word); 
                amt_conector_words++;
            }
            else {
                printf("%s(%d) ", tweet[curr_index].word, i+1);
                i++;
            }
        }
        curr_index = start_index + i + amt_conector_words;
        /*We print the word after the adjectives and adverbs that they describe
          for context for the user*/      
        printf("%s\n", tweet[curr_index].word);
        printf("Curr count:%d\n", curr_count);
}

tagged_word_tbl *clean_hotspots(int *number_of_words, tagged_word_tbl *full_tweet, int count[]) {
    int full_tweet_index, shorter_tweet_index = 0;
    tagged_word_tbl *shorter_tweet;

    shorter_tweet = (tagged_word_tbl*)malloc((*number_of_words) * sizeof(tagged_word_tbl));
    
    for (full_tweet_index = 0; full_tweet_index < *number_of_words; full_tweet_index++) {
        if (count[full_tweet_index] >= HIGH_TAG_AMOUNT) {
            full_tweet_index += determine_amount_words_to_remove(full_tweet_index, full_tweet, count[full_tweet_index]);
        }
        
        shorter_tweet[shorter_tweet_index] = full_tweet[full_tweet_index];
        shorter_tweet_index++;
    }

    *number_of_words = shorter_tweet_index;

    return shorter_tweet;
}

int determine_amount_words_to_remove(int curr_index, tagged_word_tbl *full_tweet, int curr_count) {
    int i = 0, amt_of_cc = 0, id;
    while (i < curr_count) {
        id = curr_index + amt_of_cc + i; 
        /*We have this extra bit, because connector words do not count towards the count*/
        if (strcmp(full_tweet[id].tag, "CC") == 0)
            amt_of_cc++;
        else {
            i++;
        }
    }
    //printf("amt_to_remove: %d\n", amt_to_remove);
    return curr_count + amt_of_cc - 1;
}

void print_compressed_tweet_to_file(tagged_word_tbl *shorter_tweet, int amt_of_words) {
    int i;
    FILE *ofp = fopen(COMPRESSED_WRITE_PATH, "w");
    for (i = 0; i < amt_of_words; i++) {
        fprintf(ofp, "%s ", shorter_tweet[i].word);
    }
}