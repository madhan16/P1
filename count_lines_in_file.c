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
