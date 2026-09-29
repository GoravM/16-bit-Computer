#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// max length of line
#define MAX_LEN 100

// global vars
int line_count = 0;     // global line counter used for syntax error
int err  = 0;
char globeLINE[MAX_LEN];
char fileNameNoExt[100];  // global variable for raw filename

// struct for tuple
typedef struct {
    char type[20];     // Command type like C_PUSH, C_ADD
    char arg1[50];     // First argument, e.g., segment or label
    int arg2;          // Second argument, e.g., index
} Command;

// checks if word is an Arithmetic / Logical Command
int isArithmetic(const char *word){
    return (
        strcmp(word, "add") == 0 || 
        strcmp(word, "sub") == 0 ||
        strcmp(word, "neg") == 0 || 
        strcmp(word, "eq")  == 0 ||
        strcmp(word, "gt")  == 0 || 
        strcmp(word, "lt")  == 0 ||
        strcmp(word, "and") == 0 || 
        strcmp(word, "or")  == 0 ||
        strcmp(word, "not") == 0
    );
}

void ErrorLine(){
    printf("line %-4d| Method not yet implemented\n", line_count);
    err = 1;
}

// finds which type of VM Command is given
// and parses the commands into Ctype, Carg1, index
Command parseCommand(char *line){
    Command cmd;
    char Ctype[20];
    char Carg1[50];
    int index;

    // reset values
    strcpy(cmd.type, "");
    strcpy(cmd.arg1, "");
    cmd.arg2 = -1;

    // the # of tuples/tokens given
    // ex: add ----------> 1
    // ex: push local 2 -> 3
    int numTokens = sscanf(line, "%s %s %d", Ctype, Carg1, &index);

    // if nothing is given
    if (numTokens < 1){
        strcpy(cmd.type, "C_INVALID");
        return cmd;
    }

    // Arithmetic (1 token required)
    if (isArithmetic(Ctype)) {
        if (numTokens != 1) {
            ErrorLine();
        }
        strcpy(cmd.type, "C_ARITHMETIC");
        strcpy(cmd.arg1, Ctype);
        cmd.arg2 = -1;
    }

    // Push / Pop (3 tokens required)
    else if (strcmp(Ctype, "push") == 0 || strcmp(Ctype, "pop") == 0) {
        if (numTokens != 3) {
            ErrorLine();
        }
        strcpy(cmd.type, strcmp(Ctype, "push") == 0 ? "C_PUSH" : "C_POP");
        strcpy(cmd.arg1, Carg1);
        cmd.arg2 = index;
    }

    // Label / Goto / If-goto (2 tokens required)
    else if (strcmp(Ctype, "label") == 0 || strcmp(Ctype, "goto") == 0 || strcmp(Ctype, "if-goto") == 0) {
        if (numTokens != 2) {
            ErrorLine();
        }
        if (strcmp(Ctype, "label") == 0){ 
            strcpy(cmd.type, "C_LABEL");
        }
        else if (strcmp(Ctype, "goto") == 0){ 
            strcpy(cmd.type, "C_GOTO");
        }
        else{
            strcpy(cmd.type, "C_IF");
        }
        
        strcpy(cmd.arg1, Carg1);
        cmd.arg2 = -1;
    }

    // Function / Call (3 tokens required)
    else if (strcmp(Ctype, "function") == 0 || strcmp(Ctype, "call") == 0) {
        if (numTokens != 3) {
            ErrorLine();
        }
        strcpy(cmd.type, strcmp(Ctype, "function") == 0 ? "C_FUNCTION" : "C_CALL");
        strcpy(cmd.arg1, Carg1);
        cmd.arg2 = index;
    }

    // Return (1 token required)
    else if (strcmp(Ctype, "return") == 0) {
        if (numTokens != 1) {
            ErrorLine();
        }
        strcpy(cmd.type, "C_RETURN");
        strcpy(cmd.arg1, "");
        cmd.arg2 = -1;
    }

    // Invalid command
    else {
        strcpy(cmd.type, "C_INVALID");
        strcpy(cmd.arg1, "");
        cmd.arg2 = -1;
    }

    return cmd;
}

// Writes ASM to the output file
void writeCommand(Command cmd, FILE *out){
    static int jumpCounter = 0;
    fprintf(out, "// line %d: %s\n", line_count, globeLINE);
    // Arithmetic
    if (strcmp(cmd.type, "C_ARITHMETIC") == 0){
        // add
        if (strcmp(cmd.arg1, "add") == 0){
            fprintf(out, "@SP\nAM=M-1\nD=M\nA=A-1\nM=D+M\n");
        }
        
        // sub
        else if (strcmp(cmd.arg1, "sub") == 0){
            fprintf(out, "@SP\nAM=M-1\nD=M\nA=A-1\nM=M-D\n");
        }
        
        // neg
        else if (strcmp(cmd.arg1, "neg") == 0){
            fprintf(out, "@SP\nA=M-1\nM=-M\n");
        }

        // and
        else if (strcmp(cmd.arg1, "and") == 0){
            fprintf(out, "@SP\nAM=M-1\nD=M\nA=A-1\nM=D&M\n");
        }
        
        // or
        else if (strcmp(cmd.arg1, "or") == 0){
            fprintf(out, "@SP\nAM=M-1\nD=M\nA=A-1\nM=D|M\n");
        }
        
        // not
        else if (strcmp(cmd.arg1, "not") == 0){
            fprintf(out, "@SP\nA=M-1\nM=!M\n");
        }
    
        // eq
        else if (strcmp(cmd.arg1, "eq") == 0){
            fprintf(out,
                "@SP\n"
                "AM=M-1\n"
                "D=M\n"
                "A=A-1\n"
                "D=M-D\n"
                "@TRUE_%d\n"
                "D;JEQ\n"
                "@SP\n"
                "A=M-1\n"
                "M=0\n"
                "@END_%d\n"
                "0;JMP\n"
                "(TRUE_%d)\n"
                "@SP\n"
                "A=M-1\n"
                "M=-1\n"
                "(END_%d)\n",
                jumpCounter, jumpCounter, jumpCounter, jumpCounter);
            
            jumpCounter++;
        }
        
        // gt
        else if (strcmp(cmd.arg1, "gt") == 0){
            fprintf(out,
                "@SP\n"
                "AM=M-1\n"
                "D=M\n"
                "A=A-1\n"
                "D=M-D\n"
                "@TRUE_%d\n"
                "D;JGT\n"
                "@SP\n"
                "A=M-1\n"
                "M=0\n"
                "@END_%d\n"
                "0;JMP\n"
                "(TRUE_%d)\n"
                "@SP\nA=M-1\n"
                "M=-1\n"
                "(END_%d)\n",
                jumpCounter, jumpCounter, jumpCounter, jumpCounter);
            
            jumpCounter++;
        }
        
        // lt
        else if (strcmp(cmd.arg1, "lt") == 0){
            fprintf(out,
                "@SP\n"
                "AM=M-1\n"
                "D=M\n"
                "A=A-1\n"
                "D=M-D\n"
                "@TRUE_%d\n"
                "D;JLT\n"
                "@SP\n"
                "A=M-1\n"
                "M=0\n"
                "@END_%d\n"
                "0;JMP\n"
                "(TRUE_%d)\n"
                "@SP\n"
                "A=M-1\n"
                "M=-1\n"
                "(END_%d)\n",
                jumpCounter, jumpCounter, jumpCounter, jumpCounter);
            jumpCounter++;
        }
        
        
    }

    // Push
    else if (strcmp(cmd.type, "C_PUSH") == 0){
        // local
        if (strcmp(cmd.arg1, "local") == 0){
            fprintf(out,
                "@%d\n"
                "D=A\n"
                "@LCL\n"
                "A=D+M\n"
                "D=M\n"
                "@SP\n"
                "A=M\n"
                "M=D\n"
                "@SP\n"
                "M=M+1\n", cmd.arg2);
        }
        
        // argument
        else if (strcmp(cmd.arg1, "argument") == 0){
            fprintf(out,
                "@%d\n"
                "D=A\n"
                "@ARG\n"
                "A=D+M\n"
                "D=M\n"
                "@SP\n"
                "A=M\n"
                "M=D\n"
                "@SP\n"
                "M=M+1\n", cmd.arg2);
        }

        // this
        else if (strcmp(cmd.arg1, "this") == 0){
            fprintf(out,
                "@%d\n"
                "D=A\n"
                "@THIS\n"
                "A=D+M\n"
                "D=M\n"
                "@SP\n"
                "A=M\n"
                "M=D\n"
                "@SP\n"
                "M=M+1\n", cmd.arg2);
        }

        // that
        else if (strcmp(cmd.arg1, "that") == 0){
            fprintf(out,
                "@%d\n"
                "D=A\n"
                "@THAT\n"
                "A=D+M\n"
                "D=M\n"
                "@SP\n"
                "A=M\n"
                "M=D\n"
                "@SP\n"
                "M=M+1\n", cmd.arg2);
        }

        // constant
        else if (strcmp(cmd.arg1, "constant") == 0){
            fprintf(out,
                "@%d\n"
                "D=A\n"
                "@SP\n"
                "A=M\n"
                "M=D\n"
                "@SP\n"
                "M=M+1\n", cmd.arg2);
        }
        
        // static
        else if (strcmp(cmd.arg1, "static") == 0){
            fprintf(out,
                "@%s.%d\n"
                "D=M\n"
                "@SP\n"
                "A=M\n"
                "M=D\n"
                "@SP\n"
                "M=M+1\n", fileNameNoExt, cmd.arg2);
        }

        // temp
        else if (strcmp(cmd.arg1, "temp") == 0){
            fprintf(out,
                "@%d\n"
                "D=M\n"
                "@SP\n"
                "A=M\n"
                "M=D\n"
                "@SP\n"
                "M=M+1\n", 5 + cmd.arg2);
        }

        // pointer
        else if (strcmp(cmd.arg1, "pointer") == 0){
            if (cmd.arg2 == 0) {
                fprintf(out,
                    "@THIS\n"
                    "D=M\n"
                    "@SP\n"
                    "A=M\n"
                    "M=D\n"
                    "@SP\n"
                    "M=M+1\n");
            } 
            else if (cmd.arg2 == 1) {
                fprintf(out,
                    "@THAT\n"
                    "D=M\n"
                    "@SP\n"
                    "A=M\n"
                    "M=D\n"
                    "@SP\n"
                    "M=M+1\n");
            }
        }

    }

    // Pop
    else if (strcmp(cmd.type, "C_POP") == 0){
        // local
        if (strcmp(cmd.arg1, "local") == 0){
            fprintf(out,
                "@%d\n"
                "D=A\n"
                "@LCL\n"
                "D=D+M\n"
                "@R13\n"
                "M=D\n"
                "@SP\n"
                "AM=M-1\n"
                "D=M\n"
                "@R13\n"
                "A=M\n"
                "M=D\n", cmd.arg2);
        }
        
        // argument
        else if (strcmp(cmd.arg1, "argument") == 0){
            fprintf(out,
                "@%d\n"
                "D=A\n"
                "@ARG\n"
                "D=D+M\n"
                "@R13\n"
                "M=D\n"
                "@SP\n"
                "AM=M-1\n"
                "D=M\n"
                "@R13\n"
                "A=M\n"
                "M=D\n", cmd.arg2);
        }

        // this
        else if (strcmp(cmd.arg1, "this") == 0){
            fprintf(out,
                "@%d\n"
                "D=A\n"
                "@THIS\n"
                "D=D+M\n"
                "@R13\n"
                "M=D\n"
                "@SP\n"
                "AM=M-1\n"
                "D=M\n"
                "@R13\n"
                "A=M\n"
                "M=D\n", cmd.arg2);
        }   

        // that
        else if (strcmp(cmd.arg1, "that") == 0){
            fprintf(out,
                "@%d\n"
                "D=A\n"
                "@THAT\n"
                "D=D+M\n"
                "@R13\n"
                "M=D\n"
                "@SP\n"
                "AM=M-1\n"
                "D=M\n"
                "@R13\n"
                "A=M\n"
                "M=D\n", cmd.arg2);
        }
        
        // static
        else if (strcmp(cmd.arg1, "static") == 0){
            fprintf(out,
                "@SP\n"
                "AM=M-1\n"
                "D=M\n"
                "@%s.%d\n"
                "M=D\n", fileNameNoExt, cmd.arg2);
        }

        // temp
        else if (strcmp(cmd.arg1, "temp") == 0){
            fprintf(out,
                "@SP\n"
                "AM=M-1\n"
                "D=M\n"
                "@%d\n"
                "M=D\n", 5 + cmd.arg2);
        }

        // pointer
        else if (strcmp(cmd.arg1, "pointer") == 0){
            if (cmd.arg2 == 0) {
                fprintf(out,
                    "@SP\n"
                    "AM=M-1\n"
                    "D=M\n"
                    "@THIS\n"
                    "M=D\n");
            } 
            else if (cmd.arg2 == 1) {
                fprintf(out,
                    "@SP\n"
                    "AM=M-1\n"
                    "D=M\n"
                    "@THAT\n"
                    "M=D\n");
            }
        }
    }

    // TO DO:

    // Label
    else if (strcmp(cmd.type, "C_LABEL") == 0){
        ErrorLine();
    }

    // Goto
    else if (strcmp(cmd.type, "C_GOTO") == 0){
        ErrorLine();
    }

    // If-goto
    else if (strcmp(cmd.type, "C_IF") == 0){
        ErrorLine();
    }

    // Function
    else if (strcmp(cmd.type, "C_FUNCTION") == 0){
        ErrorLine();
    }

    // Call
    else if (strcmp(cmd.type, "C_CALL") == 0){
        ErrorLine();
    }

    // Return
    else if (strcmp(cmd.type, "C_RETURN") == 0){
        ErrorLine();
    }

    else {
        printf("HOW ARE YOU HERE\n");
        ErrorLine();
    }

    fprintf(out, "\n");
}

// Main method
// drives the process (VM tranlsator)

// Main logic: 
// Constructs a Parser to handle the input file
// Constructs a Codewriter to handle the output file
// Marches through the input file, parsing each line and generating code from it

int main(int argc, char* argv[]){
    // check if file is given
    if (argc != 2){
        printf("Usage: %s <filename>\n", argv[0]);
        exit(1);
    }

    FILE *fp;
    char *filename = argv[1];   // file name
    char line[MAX_LEN];  // len of line

    

    // open input file and read from it
    fp = fopen(filename, "r");
    if (fp == NULL){
        printf("Error opening file\n");
        exit(1);
    }

    // extract file name without path or extension
    char *base = strrchr(filename, '/');  // handles paths like ./vm/SimpleAdd.vm
    if (!base) base = filename;
    else base++; // skip the '/'

    strncpy(fileNameNoExt, base, sizeof(fileNameNoExt));
    char *dot = strrchr(fileNameNoExt, '.');
    if (dot) *dot = '\0';  // remove the .vm extension


    // create output file and write to it
    FILE *out = fopen("output.txt", "w");
    if (out == NULL) {
        printf("Error creating output file\n");
        exit(1);
    }

    rewind(fp);  // Just to be safe

    printf("\n%-8s | %-25s | %-14s | %-15s | %-5s\n", "Line", "Command", "Type", "Arg1", "Arg2");
    printf("-------------------------------------------------------------------------------\n");

    while (fgets(line, sizeof(line), fp)){

        line_count++;
        err = 0;

        // if "#" appears then replace it with '\0'
        char *comment = strstr(line, "//");
        if (comment) {
            *comment = '\0';  // terminate the string before the comment
        }

        // remove leading white spaces
        char *start = line;
        while (isspace(*start)){
            start++;
        }

        if (*start == '\0'){
            continue;
        }

        start[strcspn(start, "\r\n")] = '\0';
        strcpy(line, start);

        Command cmd = parseCommand(line);
        
        if (err == 0){
            strcpy(globeLINE, line);
            writeCommand(cmd, out);

            // only for methods not yet implemented
            if (err == 0){
                printf("Line %-3d | %-25s | %-14s | %-15s | %-4d\n",
                    line_count, line, cmd.type, cmd.arg1, cmd.arg2);
            }
            else { 
                // TO-DO:
                // C_LABEL, C_GOTO, C_IF,
                // C_FUNCTION, C_CALL, C_RETURN
            }
            
        } else {
            printf("Syntax issues on line: %-3d\n", line_count);
        }

    }
    fclose(fp);
    fclose(out);
    exit(0);
}