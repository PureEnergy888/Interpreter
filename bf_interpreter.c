/**
 * @file bf_interpretor.c
 * @brief the program takes a brainfuck program and interprets and exeute it on the terminal.
 *
 * 
 *
 * version log
 * ------------
 * version 1 by PureEnergy, 22.01.2023
 */

#include <stdbool.h>
#include <stdio.h>

#define MAX_LINES 5000
#define DATA_SIZE 70000

#define SUCCESS 0
#define FAILURE 1

static char tape[MAX_LINES];

/**
 * the function takes the tape containing the program and goes over it and make it executable and exeute that program.
 *
 * @return Bool - return true if the program was interpreted successfully , false if there was problems reading the program.
 */

int execute_bf()
{
    int data[DATA_SIZE] = {0};
    int current_index = 0;
    int tracker = 0;
    int bracketCounter = 0;

    while(tape[current_index] != 0) {
        switch(tape[current_index]) {
        case '>':
            tracker++;
            break;
        case '<':
            tracker--;
            break;
        case '+':
            data[tracker]++;
            break;
        case '-':
            data[tracker]--;
            break;
        case '.':
            putchar(data[tracker]);
            break;
        case ',':
            data[tracker] = (int)getchar();
            break;
        case '[':

//run through a loop and a nested loop if it's exists, and check if there aren't any problems.

            if(data[tracker] == 0) {
                current_index++;

                while(true) {
                    if(tape[current_index] == EOF) {
                        return FAILURE;
                    }
                      
                    if (tape[current_index] == ']' && bracketCounter == 0) {
                        break;
                    } else if (tape[current_index] == ']') {
                        bracketCounter--;
                    } else if (tape[current_index] == '[') {
                        bracketCounter++;
                    }

                    current_index++;
                }
            }
            break;
        case ']':

//checks that the loop is closed.

            if(data[tracker] != 0) {
                current_index--;

                while(true) {
                    if(tape[current_index] == '[' && bracketCounter == 0) {
                        break;
                    } else if (tape[current_index] == '[') {
                        bracketCounter--;
                    } else if (tape[current_index] == ']') {
                        bracketCounter++;
                    }

                    if(current_index == -1) {
                        return FAILURE;
                    }

                    current_index--;
                    continue;
                }
            }
            break;
        }
        current_index++;
    }

//if the interpreted program is at the range of the already established data size and there was no problems the function will return true.

    return tracker != DATA_SIZE ? SUCCESS : FAILURE;
}

int main(int argc, const char * argv[])
{
    FILE *file = NULL;
    int status = 0;
    int tape_pointer = 0;
    char command = 0;

//We check if we are given the file name as an argument and check if the file isn't empty.

    if(argc != 2 || (file = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "Usage: %s filename\n", argv[0]);
        return FAILURE;
    }

//We enter all the brainfuck commands that exist in the given file and enter them into an array called tape.

    while((command = fgetc(file)) != EOF) {
        tape[tape_pointer] = command;
        tape_pointer++;
    }

    fclose(file);

    status = execute_bf();
    
    if(status == FAILURE) {
        fprintf(stderr, "Error!\n");
    }

    return status;
}
