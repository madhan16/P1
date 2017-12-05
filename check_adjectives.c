#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//SKAL HAVE LAVET NOGET SOM SØRGER FOR AT DEN IKKE PRINTER COUNT FOR ALLE ADJEKTIVERNE I STREG

#define MAX_SIZE_WORD 40
#define TAG_SIZE 4

typedef struct words {
    char word[MAX_SIZE_WORD];
    char tag[TAG_SIZE];
}word_list;

void load_tweet (FILE *fp, word_list *loaded_tweet);
int count_words (FILE *fp);
word_list split_word (char *temp);
int check_for_multiple_adjectives (int i, word_list *loaded_tweet, int number_of_words);

int main(void)
{
    int i, number_of_words, count;
    word_list *loaded_tweet;
    FILE *fp;
    fp = fopen("tweet_tagged.txt", "r");
    number_of_words = count_words(fp);

    loaded_tweet = (word_list*)malloc(number_of_words * sizeof(word_list)); 
    rewind(fp);
    load_tweet(fp, loaded_tweet);

    for(i = 0; i < number_of_words; i++){
        count = check_for_multiple_adjectives(i, loaded_tweet, number_of_words);
        printf("Word: %s_%s Count: %d\n", loaded_tweet[i].word, loaded_tweet[i].tag, count);
    }

    fclose(fp);
}

int count_words (FILE *fp) {
  char temp_word[MAX_SIZE_WORD];
  int i = 0;
  /*The 39 is MAX_WORD_SIZE -1*/
  while (fscanf(fp, "%39s", temp_word) == 1) { /*Is true while fscanf can retrieve a word*/
    i++;
  }
  return i; 
}

void load_tweet (FILE *fp, word_list *loaded_tweet) {
    char temp_word[MAX_SIZE_WORD];
    int i = 0;
    while(fscanf(fp, "%39s", temp_word) == 1) {
        loaded_tweet[i] = split_word(temp_word);
        i++;
    }
}

word_list split_word (char *temp) {
    word_list w;
    /*The number 2 is here because we are only interessted in JJ for now not JJs*/
    sscanf(temp," %[^_] _ %2s", w.word, w.tag);
    return w;
}

/*Rekursion fuck yeah boiiiiii*/
int check_for_multiple_adjectives (int i, word_list *loaded_tweet, int number_of_words) {
    if(strcmp(loaded_tweet[i].tag, "JJ") == 0 || strcmp(loaded_tweet[i].tag, "RB") == 0) {
        if (i+1 < number_of_words)
            return 1+check_for_multiple_adjectives(i+1, loaded_tweet, number_of_words);
        else 
            return 1;
    }
        
    else 
        return 0;
}
