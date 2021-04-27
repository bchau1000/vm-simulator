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

// If the user chooses FIFO, store the first page writen to main memory here
int FIFO[4] = {-1, -1, -1, -1};

// If the user chooses LRU
int LRU[4] = {-1, -1, -1, -1};
int len = 0;

/*
    page_num: if valid_bit is true/1, page_num represents its page on main memory
              if valid_bit is false/0, page_num represents the page on disk memory

    valid_bit: true/1 if page is also stored in main memory
               false/0 if page is only stored in virtual memory

    dirty_bit: if dirty_bit is true/1, page has been modified while in main memory
               if dirty_bit is false/0, page has not been modified while in main memory
               dirty_bit has no meaning if page is not in main memory (eg. valid_bit = false/0)
*/
struct Page {
    int page_num;
    bool valid_bit;
    bool dirty_bit;
};

/*
    vm_addr: object's address in virtual memory
    value: value stored in object
*/
struct Object {
    int vm_addr;
    int value;
};

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
void remove_page() {
    
    if(len > 0) {
        if(pr_algo) {
            int i;
            for(i = 0; i < 5; i++) 
                FIFO[i] = FIFO[i + 1];
    
            --len;
        }
        else {

        }
        
    }
    
}

void insert_page(int page) {
    int i;
    if(len < 4) {
        if(pr_algo) {
            FIFO[len++] = page;
        }
        else {
            LRU[len] = page;
            ++len;
        }
    }
    
    
    
}

/*
*/
void read(struct Object main_mem[MAIN_PAGES][MAX_ADDR], struct Object virt_mem[VIRT_PAGES][MAX_ADDR], 
          struct Page page_table[VIRT_PAGES], int *main_len, int addr) {
    int offset = addr % 8;
    int page = addr / 8;
    
    struct Object obj = virt_mem[page][offset];
    
    if(!page_table[page].valid_bit) {
        printf("A Page Fault Has Occurred\n");
        if((*main_len) < 4) {
            //printf("(%i) Reading value %i from page %i\n", addr, offset, page);
            //printf("(%i) value = %i\n", obj.vm_addr, obj.value);
            printf("%i\n", obj.value);
            int i;
            for(i = 0; i < MAX_ADDR; i++) 
                main_mem[*main_len][i].value = virt_mem[page][i].value;

            insert_page(page);

            page_table[page].valid_bit = true;
            page_table[page].page_num = (*main_len);
            (*main_len)++;
        }
        else {
            if(pr_algo) {
                //printf("Removed %i ", FIFO[0]);

                // Get the vm page of the old values
                int dpn = FIFO[0];
                page_table[dpn].valid_bit = false;

                // Get the new open spot in main
                int pnn = page_table[dpn].page_num;
                remove_page();
                insert_page(page);

                page_table[page].valid_bit = true;
                page_table[page].page_num = pnn;

                int i;
                for(i = 0; i < MAX_ADDR; i++) 
                    main_mem[pnn][i].value = virt_mem[page][i].value;
            }
            else {
                printf("Running LRU...");
            }
        }
    }
    else {
        //printf("(%i) Reading value %i from page %i\n", addr, offset, page);
        //printf("(%i) value = %i\n", obj.vm_addr, obj.value);
        printf("%i\n", obj.value);
    }
        
}

/*
*/
void write(struct Object main_mem[MAIN_PAGES][MAX_ADDR], struct Object virt_mem[VIRT_PAGES][MAX_ADDR], 
            struct Page page_table[VIRT_PAGES], int *main_len, int addr, int num) {
    int offset = addr % 8;
    int page = addr / 8;

    if(!page_table[page].valid_bit) {
        printf("A Page Fault Has Occurred\n");
        if((*main_len) < 4) {
            
            //printf("Writing value %i to main address %i at main page %i representing virtual page %i\n", num, offset, *main_len, page);

            virt_mem[page][offset].value = num;

            int i;
            for(i = 0; i < MAX_ADDR; i++) 
                main_mem[*main_len][i].value = virt_mem[page][i].value;

            page_table[page].valid_bit = true;
            page_table[page].page_num = (*main_len);
            (*main_len)++;
        }
        else {
            if(pr_algo) {
                // Get the vm page of the old values
                int dpn = FIFO[0];
                page_table[dpn].valid_bit = false;

                // Get the new open spot in main
                int pnn = page_table[dpn].page_num;
                remove_page();
                insert_page(page);
                virt_mem[page][offset].value = num;

                int i;
                for(i = 0; i < MAX_ADDR; i++) 
                    main_mem[pnn][i].value = virt_mem[page][i].value;

                page_table[page].valid_bit = true;
                page_table[page].page_num = pnn;
                page_table[page].dirty_bit = true;
            }
            else {
                printf("Running LRU...");
            }
        }
    }
    else {
        int pnn = page_table[page].page_num;
        page_table[page].dirty_bit = true;
        main_mem[pnn][offset].value = num;
        virt_mem[page][offset].value = num;
    }
}

/*
    Provided a physical page number, output object in the format "vm_addr:value"
        main_mem: the contents of the main memory
        ppn: physical page number
*/
void showmain(struct Object main_mem[MAIN_PAGES][MAX_ADDR], int ppn) {
    int i;
    for(i = 0; i < MAX_ADDR; i++)
        printf("%i:%i\n", main_mem[ppn][i].vm_addr, main_mem[ppn][i].value);
}

/*
    Provided a disk page number, output object in the format "vm_addr:value"
        virt_mem: the contents of the virtual memory
        dpn: disk page number
*/
void showdisk(struct Object virt_mem[MAIN_PAGES][MAX_ADDR], int dpn) {
    int i;
    for(i = 0; i < MAX_ADDR; i++)
        printf("%i:%i\n", virt_mem[dpn][i].vm_addr, virt_mem[dpn][i].value);
}

/*
    Output page table in the format "ppn:valid_bit:page_num:dirty_bit"
        page_table: the contents of the page table
*/
void showptable(struct Page page_table[VIRT_PAGES]) {
    int i;
    for(i = 0; i < VIRT_PAGES; i++)
        printf("%i:%i:%i:%i\n", i, page_table[i].valid_bit, page_table[i].page_num, page_table[i].dirty_bit);
}

/*
*/
void handleFIFO() {

}

/*
*/
void handleLRU() {

}

/*
Parameters:
    main_mem and virt_mem:  2D 'struct Object' array, first dimension represents the page,
                            each page contains 8 addresses (stored as objects)
    
    page_table: 1D 'struct Page' array, each index represents the page it belongs to in virtual memory

    VP0: Addresses 0 - 7
    VP1: Addresses 8 - 15
    VP2: Addresses 16 - 23
    VP3: Addresses 24 - 31
    VP4: Addresses 32 - 39
    VP5: Addresses 40 - 47
    VP6: Addresses 48 - 55
    VP7: Addresses 56 - 63
*/
void init_memory(struct Object main_mem[MAIN_PAGES][MAX_ADDR], struct Object virt_mem[VIRT_PAGES][MAX_ADDR], struct Page page_table[VIRT_PAGES]) {
    int i, j, z = 0;

    for(i = 0; i < MAIN_PAGES; i++) {
        for(j = 0; j < MAX_ADDR; j++) {
            main_mem[i][j].value = -1;
            main_mem[i][j].vm_addr = z++;
        }
    }

    z = 0;
    for(i = 0; i < VIRT_PAGES; i++) {
        for(j = 0; j < MAX_ADDR; j++) {
            virt_mem[i][j].value = z;
            virt_mem[i][j].vm_addr = z++;
        }
    }

    for(i = 0; i < VIRT_PAGES; i++) {
        page_table[i].page_num = i;
        page_table[i].valid_bit = false;
        page_table[i].dirty_bit = false;
    }
}



int main(int argc, char *argv[]) {
    bool quit = false;
    
    struct Object main_mem[MAIN_PAGES][MAX_ADDR];
    struct Object virt_mem[VIRT_PAGES][MAX_ADDR];
    int main_len = 0;

    struct Page page_table[VIRT_PAGES];

    init_memory(main_mem, virt_mem, page_table);

    if(argc > 1) {
        if(strncmp("FIFO", argv[1], 4) == 0)
            pr_algo = true;
        else if(strncmp("LRU", argv[1], 3) == 0)
            pr_algo = false;
    }

    while(!quit) {
        char raw_input[MAX_INPUT] = "\0";
        char *input[MAX_ARG_C] = {"\0"};

        printf("$> ");
        fgets(raw_input, MAX_INPUT, stdin);
        parseInput(input, raw_input);

        if(strncmp(input[0], "quit", 4) == 0) {
            printf("Exiting VM-Simulator...\n");
            quit = true;
        } 
        else if(strncmp(input[0], "read", 4) == -0) {
            if(input[1]) {
                int addr = atoi(input[1]);

                if(addr >= 0 && addr < 64) {
                    read(main_mem, virt_mem, page_table, &main_len, addr);
                }
                else
                    printf("Address out of bounds, must be between 0 and 63\n");
            }
            else 
                printf("Must provide an address to read\n");
        }
        else if(strncmp(input[0], "write", 5) == 0) {
            if(input[1] && input[2]) {
                int addr = atoi(input[1]);
                int num = atoi(input[2]);

                if(addr >= 0 && addr < 64) {
                    
                    int value = atoi(input[2]);
                    write(main_mem, virt_mem, page_table, &main_len, addr, num);
                }
                else
                    printf("Address out of bounds, must be between 0 and 63\n");
            }
            else 
                printf("Must provide an address and value to write\n");
        }
        else if(strncmp(input[0], "showmain", 8) == 0) {
            if(input[1]) {
                int ppn = atoi(input[1]);

                if(ppn >= 0 && ppn < 4)
                    showmain(main_mem, ppn);
                else 
                    printf("Must provide a physical page number between 0 and 3\n");
            }
            else
                printf("Must provide a physical page number to show\n");
        }
        else if(strncmp(input[0], "showdisk", 8) == 0) {
            if(input[1]) {
                int dpn = atoi(input[1]);

                if(dpn >= 0 && dpn < 8)
                    showdisk(virt_mem, dpn);
                else 
                    printf("Must provide a disk page number between 0 and 7\n");
            }
            else
                printf("Must provide a disk page number to show\n");
        }
        else if(strncmp(input[0], "showptable", 10) == 0) {
            showptable(page_table);
        }
        else
            printf("Command '%s' not found.\n", input[0]);
            
        }

}

/*
    

*/