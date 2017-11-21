#include <stdio.h>
#include <ctype.h>
#include <string.h>
#define MAX_TEXT_LENGHT 280
#define MAX_SIZE_WORD 45
#define MAX_AMOUNT_WORDS 20

int SplitToWords(char *inputText, char outputArray[][MAX_SIZE_WORD]);

int main() {
   char str[MAX_TEXT_LENGHT];
   char splitStrings[MAX_AMOUNT_WORDS][MAX_SIZE_WORD]; // 20 ord, 45 bogstaver
   int count;
 
   printf("Enter a string: ");
   gets(str);
   
   count = SplitToWords(str, splitStrings);
   
   printf("\nOriginal String is: %s", str);
   printf("\nYour string is seperated into:\n");
   printf("Value of count: %d\n", count);
   for(int i=0; i < count; i++)
      printf("%s\n", splitStrings[i]);
   
   return 0;
}

int SplitToWords(char *inputText, char outputArray[][MAX_SIZE_WORD]) {
   int i, j=0, count=0;

   for(i=0; i<=(strlen(inputText)); i++) {    
      if (isalnum(inputText[i])) 
         outputArray[count][j++] = inputText[i];
      else if (inputText[i]==' '|| inputText[i]=='\0') {
         outputArray[count][j] = '\0';
         count++;
         j=0;
         }
      else {
         outputArray[count][j] = '\0';
         count++;
         j=0;
         outputArray[count][j] = inputText[i];
         j++;
      }
    }
   printf("Value of count in function: %d\n", count);
   return count;
}
