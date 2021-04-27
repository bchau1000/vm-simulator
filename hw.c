#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <stdbool.h>

#define MAX_INPUT 100
#define MAX_ARG_C 10

#define MAX_ADDR 8
#define MAIN_PAGES 4
#define VIRT_PAGES 8

void getArgs(char *input[], char raw_input[]) {
    char *stripNewline = strchr(raw_input, '\n');
    if (stripNewline)
        *stripNewline = 0;

    char *token = strtok(raw_input, " ");
    char **inputPtr = input;

    while(token != NULL) {
        *inputPtr++ = token;
        token = strtok(NULL, " ");
    }
}

/*
    Initialize all memory addresses to -1
    Initialize page_table all to false/0, all pages are initially on disk/virtual memory
*/
void init_memory(int main_mem[MAIN_PAGES][MAX_ADDR], int virt_mem[VIRT_PAGES][MAX_ADDR],
    bool page_table[VIRT_PAGES]) {
    int i, j;

    for(i = 0; i < MAIN_PAGES; i++) 
        for(j = 0; j < MAX_ADDR; j++)
            main_mem[i][j] = -1;

    for(i = 0; i < VIRT_PAGES; i++) 
        for(j = 0; j < MAX_ADDR; j++)
            virt_mem[i][j] = -1;
    
    for(i = 0; i < VIRT_PAGES; i++)
        page_table[i] = false;
}

/*
Parameters:
    main_mem and virt_mem:  2D integer array, first dimension represents the page,
                            each page contains 8 addresses (stored as integers)
    
    page_table: 1D boolean array, each index represents a page, the boolean
                value tells us if the page exists in physical memory (true/1)
                or only exists in virtual memory (false/0)

                eg. page hit == 1, page fault = 0
*/
int main(int argc, char *argv[]) {
    char raw_input[MAX_INPUT] = "\0";
    char *input[MAX_ARG_C] = "\0";
    bool exit = false;
    
    int main_mem[MAIN_PAGES][MAX_ADDR] = {{-1}};
    int virt_mem[VIRT_PAGES][MAX_ADDR] = {{-1}};
    bool page_table[MAIN_PAGES] = {0};
    
    init_memory(main_mem, virt_mem, page_table);

    while(!exit) {
        printf("> ");
        fgets(raw_input, MAX_ARG_C, stdin);
        getArgs()
    }

    return 0;
}

/*
if(strncmp(argv[1], "read", 4) == -0) {

}
else if(strncmp(argv[1], "write", 5) == 0) {
}
else if(strncmp(argv[1], "showdisk", 8) == 0) {
}
else if(strncmp(argv[1], "showptable", 10) == 0) {
}
else if(strncmp(argv[1], "quit", 4) == 0) {
}
else
    printf("Command %s not found.", argv[1]);
*/
