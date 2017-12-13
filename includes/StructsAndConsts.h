#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE 200
#define MAX_WORD_LGT 50
#define MAX_AMT_WORDS_IN_TWEET 280
#define MAX_AMT_SIGNS_IN_TWEET 1000
#define AMT_SYN 100
#define TAG_SIZE 4
#define HIGH_TAG_AMOUNT 2
#define MAX_AMT_SPECIAL_SIGNS 15

/*change to path /WIN/ if windows*/
/*change to path /MAC/ if mac*/
#define TWEET_PATH "../long_tweet.txt"
#define ABB_PATH "../files/WIN/shortened.txt"
#define SYNONYM_PATH "../files/WIN/synonymdatabase.txt"
#define UNTAGGED_WRITE_PATH "untagged_tweet.txt"
#define TAGGED_TWEET_PATH "tweet_tagged.txt" 
#define COMPRESSED_WRITE_PATH "../compressed.txt"


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
    char synonym[AMT_SYN][MAX_WORD_LGT];
} synonym_tbl;

typedef struct tagged_word {
    char word[MAX_WORD_LGT];
    char tag[TAG_SIZE];
} tagged_word_tbl;

typedef struct special_signs {
    char sign[MAX_AMT_SPECIAL_SIGNS];
    int size;
} special_signs_tbl;