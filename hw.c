#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <stdbool.h>

#define MAX_INPUT 100
#define MAX_ARG_C 10

#define MAX_ADDR 8
#define MAIN_PAGES 4
#define VIRT_PAGES 8

// Page Replacement Algorithm = pr_algo
// FIFO: pr_algo = true (default)
// LRU: pr_algo = false
bool pr_algo = true;

// If the user chooses FIFO, store the first page wrriten to main memory here
int FIFO[1] = {-1};

/*
    Tokenize raw_input[] and store in *input[]
    input[0] stores the command itself
    input[1...] will store the various arguments, if any
*/
void parseInput(char *input[], char raw_input[]) {
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
*/
void read(int virt_mem[VIRT_PAGES][MAX_ADDR], bool page_table[VIRT_PAGES], int addr) {
    
}

/*
*/
void write(int virt_mem[VIRT_PAGES][MAX_ADDR], bool page_table[VIRT_PAGES], int addr, int num) {

}

/*
*/
void showmain() {

}

/*
*/
void showdisk() {

}

/*
*/
void showptable() {

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

    VP0: Addresses 0 - 7
    VP1: Addresses 8 - 15
    VP2: Addresses 16 - 23
    VP3: Addresses 24 - 31
    VP4: Addresses 32 - 39
    VP5: Addresses 40 - 47
    VP6: Addresses 48 - 55
    VP7: Addresses 56 - 63
*/
int main(int argc, char *argv[]) {
    char raw_input[MAX_INPUT] = "\0";
    char *input[MAX_ARG_C] = {"\0"};
    bool quit = false;
    
    int main_mem[MAIN_PAGES][MAX_ADDR] = {{-1}};
    int virt_mem[VIRT_PAGES][MAX_ADDR] = {{-1}};
    bool page_table[MAIN_PAGES] = {0};
    init_memory(main_mem, virt_mem, page_table);

    // Page Replacement Algorithm = pr_algo
    // FIFO: pr_algo = true (default)
    // LRU: pr_algo = false
    if(argc > 1) {
        if(strncmp("FIFO", argv[1], 4) == 0)
            pr_algo = true;
        else if(strncmp("LRU", argv[1], 3) == 0)
            pr_algo = false;
    }

    while(!quit) {
        printf("$> ");
        fgets(raw_input, MAX_INPUT, stdin);
        parseInput(input, raw_input);

        if(strncmp(input[0], "quit", 4) == 0) {
            printf("Exiting VM-Simulator...\n");
            quit = true;
        } 
        else if(strncmp(input[0], "read", 4) == -0) {
        }
        else if(strncmp(input[0], "write", 5) == 0) {
        }
        else if(strncmp(input[0], "showdisk", 8) == 0) {
        }
        else if(strncmp(input[0], "showptable", 10) == 0) {
        }
        else
            printf("Command '%s' not found.\n", input[0]);
        }

    return 0;
}
