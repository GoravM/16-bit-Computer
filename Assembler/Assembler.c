#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// all comp syntax and binaries
const char *comp1[] = {
    "0", "1", "-1", "D", "A", "M", 
    "!D", "!A", "!M", "-D", "-A", "-M", 
    "D+1", "A+1", "M+1", "D-1", "A-1", "M-1", 
    "D+A", "D+M", "D-A", "D-M", "A-D", "M-D", 
    "D&A", "D&M", "D|A", "D|M"
};
const char *comp2[] = {
    "0101010", // 0
    "0111111", // 1
    "0111010", // -1
    "0001100", // D
    "0110000", // A
    "1110000", // M
    "0001101", // !D
    "0110001", // !A
    "1110001", // !M
    "0001111", // -D
    "0110011", // -A
    "1110011", // -M
    "0011111", // D+1
    "0110111", // A+1
    "1110111", // M+1
    "0001110", // D-1
    "0110010", // A-1
    "1110010", // M-1
    "0000010", // D+A
    "1000010", // D+M
    "0010011", // D-A
    "1010011", // D-M
    "0000111", // A-D
    "1000111", // M-D
    "0000000", // D&A
    "1000000", // D&M
    "0010101", // D|A
    "1010101"  // D|M
};

// all dest syntax and binaries
const char *dest1[] = {"", "M", "D", "MD", "A", "AM", "AD", "AMD"};
const char *dest2[] = {
    "000", // null
    "001", // M
    "010", // D
    "011", // MD
    "100", // A
    "101", // AM
    "110", // AD
    "111"  // AMD
};

// all jump syntax and binaries
const char *jump1[] = {"", "JGT", "JEQ", "JGE", "JLT", "JNE", "JLE", "JMP"};
const char *jump2[] = {
    "000", // null
    "001", // JGT
    "010", // JEQ
    "011", // JGE
    "100", // JLT
    "101", // JNE
    "110", // JLE
    "111"  // JMP
};

// Global Binaries
char opcode_bit[2];
char ignore[3] = "00"; // unused bits ignore
char comp_bits[8];
char dest_bits[4];
char jump_bits[4];

// global line counter to know which line has an error
int line_count = 0;

// Final Binary for output
char Final_Binary[17] = "";

// Struct for Labels
#define MAX_SYMBOLS 500

typedef struct  {
    char name[100];
    int address;
} Symbol;

Symbol symbol_table[MAX_SYMBOLS];
int symbol_count = 0;

// add Symbol Method
void add_symbol(const char *name, int address) {
    // Check if the symbol already exists to avoid duplicates
    for (int i = 0; i < symbol_count; i++) {
        if (strcmp(symbol_table[i].name, name) == 0) {
            return; // already exists, do nothing
        }
    }

    // Add new symbol to table
    strcpy(symbol_table[symbol_count].name, name);
    symbol_table[symbol_count].address = address;
    symbol_count++;
}

// get address of symbol 
// @symbol -> @address
int get_address(const char *name) {
    for (int i = 0; i < symbol_count; i++) {
        if (strcmp(symbol_table[i].name, name) == 0) {
            return symbol_table[i].address;
        }
    }
    return -1; // Not found
}

// Load Predefined Symbols
void load_predefined_symbols() {
    char r[4];
    for (int i = 0; i <= 15; i++) {
        sprintf(r, "R%d", i);
        add_symbol(r, i);
    }
    add_symbol("SP", 0);
    add_symbol("LCL", 1);
    add_symbol("ARG", 2);
    add_symbol("THIS", 3);
    add_symbol("THAT", 4);
    add_symbol("SCREEN", 16384);
    add_symbol("KBD", 24576);
}

// set everything to 0
void reset_bits(){
    strcpy(Final_Binary, "");
    strcpy(opcode_bit, "0");
    strcpy(ignore, "00");
    strcpy(comp_bits, "0000000");
    strcpy(dest_bits, "000");
    strcpy(jump_bits, "000");
}

// method for testing
void print_symbol_table() {
    printf("\n--- SYMBOL TABLE ---\n");
    for (int i = 0; i < symbol_count; i++) {
        printf("%s -> %d\n", symbol_table[i].name, symbol_table[i].address);
    }
    printf("--------------------\n");
}

// method for testing
void printGlobals() {
    printf("Opcode: %s, Ignore: %s, Comp: %s, Dest: %s, Jump: %s\n", opcode_bit, ignore, comp_bits, dest_bits,jump_bits);
}

// method for checking if string is a number
int is_number(const char *str){
    if (str[0] == '\0'){
        return 0;
    }

    for (int i = 0; str[i] != '\0'; i++){
        if (!isdigit(str[i])){
            return 0;
        }
    }
    return 1;
}

// For A-Instruction: @value -> 101001 (translator)
void toBinary16(int num, char binary[17]) {
    for (int i = 15; i >= 0; i--) {
        int bit = (num >> i) & 1;       // Shift right and mask the last bit

        if (bit == 1) {
            binary[15 - i] = '1';
        } else {
            binary[15 - i] = '0';
        }
    }

    binary[16] = '\0'; // null-terminate the string
}

// For A-Instruction: binary -> global binary
void BintoGlobal(char Binary[17]){
    strncpy(opcode_bit, Binary, 1);
    opcode_bit[1] = '\0';

    // dont ignore bc its A-Instruction
    strncpy(ignore, Binary + 1, 2);
    ignore[2] = '\0';

    strncpy(comp_bits, Binary + 3, 7);
    comp_bits[7] = '\0';

    strncpy(dest_bits, Binary + 10, 3);
    dest_bits[3] = '\0';

    strncpy(jump_bits, Binary + 13, 3);
    jump_bits[3] = '\0';
}

// remove spaces, '\n' and '\t'
void removeSpaces(char *str) {
    int i = 0, j = 0;
    while (str[i]) {
        if (str[i] != ' ' && str[i] != '\n' && str[i] != '\t') {
            str[j++] = str[i];
        }
        i++;
    }
    str[j] = '\0';
}

// removes brackets
void removeBrackets(char *str) {
    int i = 0, j = 0;
    while (str[i]) {
        if (str[i] != '(' && str[i] != ')') {
            str[j++] = str[i];
        }
        i++;
    }
    str[j] = '\0';
}

// method for testing
void printstrs(char dest_str[10], char comp_str[10], char jump_str[10]){
    printf("dest_str: %s, comp_str: %s, jump_str: %s\n", dest_str, comp_str, jump_str);
}

// methods to make syntax to binary in respective binary parts
// Iterates through respective arrays and checks if str is in it
// makes the global binaries values become the str's translated binary value
// in their respective parts
void desttobin(char dest_str[10]) {
    int found = 0;
    for (int i = 0; i < 8; i++) {
        if (strcmp(dest_str, dest1[i]) == 0) {
            strcpy(dest_bits, dest2[i]);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Syntax issue on line: %d\n", line_count);
        exit(1);
    }
}

void comptobin(char comp_str[10]) {
    int found = 0;
    for (int i = 0; i < 28; i++) {
        if (strcmp(comp_str, comp1[i]) == 0) {
            strcpy(comp_bits, comp2[i]);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Syntax issue on line: %d\n", line_count);
        exit(1);
    }
}

void jumptobin(char jump_str[10]) {
    int found = 0;
    for (int i = 0; i < 8; i++) {
        if (strcmp(jump_str, jump1[i]) == 0) {
            strcpy(jump_bits, jump2[i]);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Syntax issue on line: %d\n", line_count);
        exit(1);
    }
}

// Builds 16-bit output for line
void buildFinalBinary() {
    // Reset
    strcpy(Final_Binary, "");

    strcat(Final_Binary, opcode_bit);
    strcat(Final_Binary, ignore);
    strcat(Final_Binary, comp_bits);
    strcat(Final_Binary, dest_bits);
    strcat(Final_Binary, jump_bits);
}



int main(int argc, char *argv[]){
    // check if file is given
    if (argc != 2){
        printf("Usage: %s <filename>\n", argv[0]);
        exit(1);
    }

    FILE *fp;
    char *filename = argv[1];   // file name
    char line[100];  // len of line

    // open input file and read from it
    fp = fopen(filename, "r");
    // if not found
    if (fp == NULL){
        printf("Error opening file\n");
        exit(1);
    }

    // create output file and write to it
    FILE *out = fopen("output.txt", "w");
    if (out == NULL) {
        printf("Error creating output file\n");
        exit(1);
    }

    // First Pass: Label Checks and Storing ------------------------------------------------------------
    // Scans the file to identify and store label declarations with their respective ROM addresses.
    load_predefined_symbols();

    int rom_address = 0;
    rewind(fp);  // Just to be safe

    while (fgets(line, sizeof(line), fp)) {

        //increment instruction (ROM) address
        rom_address++;

        // remove comments
        char *comment = strstr(line, "//");
        if (comment) *comment = '\0';

        removeSpaces(line);

        //printf("line %d: %s\n",rom_address, line);

        // OPTIONAL but have to add the same line in second pass if using this
        // skip empty lines
        // if (line[0] == '\0') continue;

        // label check
        if (line[0] == '(') {
            int len = strlen(line);
            if (line[len - 1] != ')'){
                printf("Flag Syntax issue on line: %d\n", rom_address);
                exit(1);
            }
            

            char label[100];
            //printf("LABEL DETECTED\n");
            // remove brackets
            removeBrackets(line);
            strcpy(label, line);

            //printf("Label: %s, stored at line: %d\n", label, rom_address);
            add_symbol(label, rom_address);  // store label with current ROM address
            continue;
        }

        
    }


    // Second Pass: Instruction Translation -----------------------------------------------------------
    // Translates A- and C-instructions into binary, resolves symbols, and allocates variable addresses

    // Empty Global Binaries
    reset_bits();

    // Go Through File Again
    rewind(fp);
 
    // go through file
    while (fgets(line, sizeof(line), fp)){

        // line counter
        line_count++;

        // if "//" appears then replace it with '\0'
        char *comment = strstr(line, "//");
        if (comment) {
            *comment = '\0';  // terminate the string before the comment
        }

        // remove spaces
        removeSpaces(line);

        // OPTIONAL
        // empty lines and comment lines get treated as "@0" line that does nothing
        if (line[0] == '\0'){
            //printf("skipped line: %d\n", line_count);
            strcpy(Final_Binary, "0000000000000000");
            //printf("%s\n", Final_Binary);

            // OPTINAL NOT NEEDED
            // but for code to work when using 
            // flags and loops (in specific lines) it is NEEDED
            fprintf(out, "%s\n", Final_Binary);

            //printf("\n");
            continue;
        }

        // if flag then ignore it and treat it as @0
        if (line[0] == '('){
            strcpy(Final_Binary, "0000000000000000");
            fprintf(out, "%s\n", Final_Binary);
            continue;
        }

        //printf("line %d: %s\n", line_count, line);

        // find which index '=' and ';' occurs on 
        // bc dest && jump syntax can be null
        int equal_indx = -1;
        int equal_count = 0;
        int semi_indx = -1;
        int semi_count = 0;
        for (int i = 0; line[i] != '\0'; i++){
            if (line[i] == '='){
                equal_indx = i;
                equal_count++;
            }
            if (line[i] == ';'){
                semi_indx = i;
                semi_count++;
            }
        }

        // printf("equal %d, semi %d: \n", equal_indx, semi_indx);

        // edge cases
        if (line[0] == '@' && (equal_indx > -1 || semi_indx > -1)){
            printf("Syntax issues on line: %d\n", line_count);
            exit(1);
        }

        else if (equal_count > 1 || semi_count > 1){
            printf("Syntax issues on line: %d\n", line_count);
            exit(1);
        }

        // A-Instruction
        else if (line[0] == '@'){
            strcpy(opcode_bit, "0");
            char symbol[100];
            strcpy(symbol, line + 1); // @symbol -> symbol

            // its either address or number
            int address;

            // if symbol are numbers
            if (is_number(symbol)){
                address = atoi(symbol);
            } 
            // else its a symbol
            else {
                // check iff there is an address
                address = get_address(symbol);

                // if not in symbol table
                // make it into a variable
                if (address == -1){
                    static int ram_address = 16;
                    address = ram_address;
                    add_symbol(symbol, ram_address++); // also post-increment so value is up next time
                }
            }


            char Abin[17];
            toBinary16(address, Abin);

            // change global binary into Abin
            BintoGlobal(Abin);
        }

        // C-Instruction set opcode = 1
        else {

            //printf("in C-instr line: %s\n", line);

            strcpy(opcode_bit, "1");

            // sub section strings
            char comp_str[10] = "";
            char dest_str[10] = "";
            char jump_str[10] = "";

            // Format: dest=comp;jump
            if (equal_indx != -1 && semi_indx != -1){
                // printf("Format: 1\n");

                int lenbet = semi_indx - equal_indx - 1;

                strncpy(dest_str, line, equal_indx);
                dest_str[equal_indx] = '\0';

                strncpy(comp_str, line + equal_indx + 1, lenbet);
                comp_str[strcspn(comp_str, "\n")] = '\0';

                strncpy(jump_str, line + semi_indx + 1, strlen(line + semi_indx + 1));
                jump_str[strcspn(jump_str, "\n")] = '\0'; // strip newline
            }
            
            // Format: dest=comp
            else if(equal_indx != -1){
                // printf("Format: 2\n");

                strncpy(dest_str, line, equal_indx);
                dest_str[equal_indx] = '\0';

                strcpy(comp_str, line + equal_indx + 1);
                comp_str[strcspn(comp_str, "\n")] = '\0';

            }

            // Format: comp;jump
            else if (semi_indx != -1){
                // printf("Format: 3\n");

                // Copy comp part: from start up to ;
                strncpy(comp_str, line, semi_indx);
                comp_str[semi_indx] = '\0';

                // Copy jump part: after ;
                strncpy(jump_str, line + semi_indx + 1, strlen(line + semi_indx + 1));
                jump_str[strcspn(jump_str, "\n")] = '\0'; // strip newline
            }

            // Format: comp
            else if (equal_indx == -1 && semi_indx == -1) {
                // printf("Format: 4\n");
                strncpy(comp_str, line, strlen(line));
                comp_str[strcspn(comp_str, "\n")] = '\0';
            }

            // Invalid Case
            else {
                // printf("Format: Uh oh\n");
                printf("Syntax issue on line: %d\n", line_count);
                exit(1);
            }
            
            // dest_str && comp_str && jump_str is done

            // if bypass then check if in arr1
            // check if in arr1 then the index gives the binary in arr2
            // if not then error
            //printstrs(dest_str, comp_str, jump_str);

            // send for translation
            desttobin(dest_str);
            comptobin(comp_str);
            jumptobin(jump_str);

        }

        //printGlobals();
        buildFinalBinary();
        //printf("%s\n", Final_Binary);
        fprintf(out, "%s\n", Final_Binary);  // write final binary to output file
        reset_bits();
        //printf("\n");
    }
    
    //print_symbol_table();
    
    // close files and exit program
    fclose(fp);
    fclose(out);
    exit(0);
}
