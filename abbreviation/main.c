/*reads abbreviations from file and puts into array of structs*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 200

typedef struct abb {
    char word[40];
    char abb[45];
    int  n_words;
} abb_tbl;

int count_lines_in_file(FILE *ifp);
void fill_array_from_file(FILE *ifp, abb_tbl *arr, int lines);
abb_tbl read_file_line(FILE *ifp);
int count_words_in_string(char *str);

int main(void) {
    int i;
    FILE *ifp = fopen("shortened.txt", "r");
    int lines_in_file;
    abb_tbl *abb_arr; 
    
    if(ifp == NULL) {
        printf("Could not connect to file\n");
        return EXIT_FAILURE;
    }
    
    lines_in_file = count_lines_in_file(ifp);
    abb_arr = (abb_tbl*)malloc(lines_in_file * sizeof(abb_tbl));
    
    if(abb_arr == NULL) {
        printf("Error allocation memory\n");
        return EXIT_FAILURE;
    }
    
    fill_array_from_file(ifp, abb_arr, lines_in_file);
    
    for(i = 0; i < lines_in_file; i++)
        printf("%s %s %d\n", abb_arr[i].word, abb_arr[i].abb, abb_arr[i].n_words);
    
    free(abb_arr);
    fclose(ifp);
    return 0;
}

/*takes a file points and returns number of lines in file*/
int count_lines_in_file(FILE *ifp) {
    int lines = 0;
    char temp_str[MAX_LINE];
    
    while(fgets(temp_str, MAX_LINE, ifp)) {
        lines++;
    }
    rewind(ifp);
    
    return lines;
}

/*takes a file pointer, array of structs and size of array and fill 
  array of struct from file*/
void fill_array_from_file(FILE *ifp, abb_tbl *arr, int lines) {
    int i;
    for(i = 0; i < lines; i++)
        arr[i] = read_file_line(ifp);
}

abb_tbl read_file_line(FILE *ifp) {
    abb_tbl p;
    
    fscanf(ifp, " %[^:] : %[^\n] ", p.word, p.abb);
    p.n_words = count_words_in_string(p.word);
    
    return p;
}

/*returns number of words in string*/
int count_words_in_string(char *str) {
    int i, count = 0;
    
    for(i = 0; i < strlen(str); i++) {
        if(str[i] == ' ')
            count++;
    }
    return count + 1;
}
