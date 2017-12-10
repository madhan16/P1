#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE 200
#define MAX_WORD_LGT 50
#define MAX_AMT_WORDS_IN_TWEET 100
#define AMT_SYN 100
#define TAG_SIZE 4
#define HIGH_TAG_AMOUNT 2

/*change to path /WIN/ if windows*/
/*change to path /MAC/ if mac*/
#define ABB_PATH "../files/WIN/shortened.txt"
#define SYNONYM_PATH "../files/WIN/synonymdatabase.txt"
#define ABB_WRITE_PATH "abbreviated_tweet.txt"
#define TAGGED_TWEET_PATH "tweet_tagged.txt" 

typedef struct word {
    char word[MAX_WORD_LGT];
    int  is_capital;
    int  is_twittertag;
} twitter_words_tbl;

typedef struct abb {
    char word[MAX_WORD_LGT];
    char abb[MAX_WORD_LGT];
    int  n_words;
} abb_tbl;

typedef struct synonyms {
    char word[MAX_WORD_LGT];
    char synonym[MAX_WORD_LGT];
} synonym_tbl;

typedef struct tagged_word {
    char word[MAX_WORD_LGT];
    char tag[TAG_SIZE];
} tagged_word_tbl;
