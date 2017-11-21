#include <stdio.h>
#include <ctype.h>
#include <string.h>
#define MAX_TEXT_LENGTH 280
#define MAX_SIZE_WORD 45
#define MAX_AMOUNT_WORDS 20

int SplitToWords(char *inputText, char outputArray[][MAX_SIZE_WORD]);
void Splitter(char outputArray[][MAX_SIZE_WORD], int *count, int *lenghtword);
void TransferCharacter(char outputArray[][MAX_SIZE_WORD], int *count, int *lenghtword, char inputChar);

int main() {
   char str[MAX_TEXT_LENGTH];
   char splitStrings[MAX_AMOUNT_WORDS][MAX_SIZE_WORD]; // 20 ord, 45 bogstaver
   int count;
 
   printf("Enter a string: ");
   gets(str);
   
   count = SplitToWords(str, splitStrings);
   
   printf("\nOriginal String is: %s", str);
   printf("\nYour string is seperated into:\n");


   for(int i = 0; i < count; i++)
      printf("%s\n", splitStrings[i]);
   
   return 0;
}

/* This is our overall function for splitting up a text into individual strings, 
   which we need for when compareing the words with our database.

   It takes takes an inputText string, as first parameter, and an two dimensional 
   array which the indvidual strings are stored in*/
int SplitToWords(char *inputText, char outputArray[][MAX_SIZE_WORD]) {
   int i, j = 0, WordCount = 0;

   for(i=0; i<=(strlen(inputText)); i++) {    
      if (isalnum(inputText[i])) 
         TransferCharacter(outputArray, &WordCount, &j, inputText[i]);
      else if (inputText[i] == ' '|| inputText[i] == '\0') {
         Splitter(outputArray, &WordCount, &j);
      }
      else {
         Splitter(outputArray, &WordCount, &j);
         TransferCharacter(outputArray, &WordCount, &j, inputText[i]);
      }
    }
   return WordCount;
}

/*Pretty sure there is a smarter way to transfer over the 2D array, than transfering it and it coordinates over in 3 bits*/
void Splitter(char outputArray[][MAX_SIZE_WORD], int *WordCount, int *lenghtword) {
   outputArray[*WordCount][*lenghtword] = '\0';
   *lenghtword = 0;
   (*WordCount)++;
}

void TransferCharacter(char outputArray[][MAX_SIZE_WORD], int *WordCount, int *lenghtword, char inputChar) {
   outputArray[*WordCount][*lenghtword] = inputChar;
   (*lenghtword)++;
}
