#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include "aggregate.h"
#include "singular.h"
#define MAX_COL 256
#define MAX_CHAR 2048   // Safe estimate of max buffer size
#define LENGTH(x) (sizeof(x)/sizeof(x[0]))

int output_size = 3;   // Print 3 digits by default
int output_prec = 1;   // Print 1 digits after the decimal by default

void setFinalCommand(char *finalCommand, char *command, int size) {
    finalCommand = realloc(finalCommand, size * sizeof(char));
    for (int i=0; i < size; i++) {
        finalCommand[i] = command[i];
    }
}

int isDouble(const char* str) {
    char* endptr = 0;
    strtod(str, &endptr);

    if(*endptr != '\0' || endptr == str)
        return 0;
    return 1;
}

int isInt(const char* str) {
    char* endptr;
    int errno = 0;

    long val = strtol(str, &endptr, 10);

    if (errno != 0 || endptr == str) {
        // An error occurred during conversion or no digits were parsed
        return 0;
    }

    // Check if any non-whitespace characters follow the number
    while (*endptr != '\0') {
        if (!isspace(*endptr)) {
            return 0;
        }
        endptr++;
    }

    // Check if the value is within the range of an int
    if (val < INT_MIN || val > INT_MAX) {
        return 0;
    }

    return 1;
}

int my_strlen(char* str) {
    int i;
    for (i=0; str[i] != '\0'; i++);
    return i;
}

void strToUpper(char* str) {
    for (int i=0; str[i] != '\0'; i++) {
        str[i] = toupper(str[i]);
    }
}

int main(int argc, char *argv[])  {
    /* ----- Dynamically allocate the contents of stdin to a 2d array ----- */
    char input[MAX_CHAR];
    char delimiter[] = " \n";
    double **arr = (double **)malloc(1 * sizeof(double *));
    int *sizes = (int *)malloc(1 * sizeof(int));
    double *innerArr;

    int row = 0;
    while (fgets(input, sizeof(input), stdin) != NULL) {
        if (strcmp(input, "\n") == 0) {continue;}
        innerArr = (double *)malloc(MAX_COL * sizeof(double));
        if (innerArr == NULL) {
            printf("Failed to allocate memory for the array.\n");
            return 1;
        }

        int col = 0;
        double scanned;
        char* token = strtok(input, delimiter);
        while (token != NULL) {
            if (col == 256) {
                arr[row] = innerArr;
                sizes[row] = col;  
                row++;
                sizes = (int *)realloc(sizes, (row + 1) * sizeof(int));
                arr = (double **)realloc(arr, (row + 1) * sizeof(double *));
                col = 0;
                innerArr = (double *)malloc(MAX_COL * sizeof(double));
                if (innerArr == NULL) {
                    printf("Failed to allocate memory for the array.\n");
                    return 1;
                }
            }
            sscanf(token, "%lf", &scanned);
            innerArr[col] = scanned;
            col++;

            token = strtok(NULL, delimiter);
        }

        innerArr = (double *)realloc(innerArr, (col) * sizeof(double));
        if (innerArr == NULL) {
            printf("Failed to reallocate memory for the array.\n");
            return 1;
        }
        arr[row] = innerArr;
        sizes[row] = col;  
        row++;
        sizes = (int *)realloc(sizes, (row + 1) * sizeof(int));
        arr = (double **)realloc(arr, (row + 1) * sizeof(double *));
    }

    /* ----- Parse the command line arguments ----- */

    //const enum commands {COUNT, MIN, MAX, SUM, AVG, PAVG, PRINT, FILTER, SHIFT} command;
    const char *validAggregateCommands[] = {"COUNT", "MIN", "MAX", "SUM", "AVG", "PAVG"};

    extern char *filter_types[];
    extern int filter_types_count;
    double threshold;
    enum filter_type t;
    double shiftValue;
    char *finalCommand = (char *)malloc(sizeof(char));
    int commandFlag = 0;
    int aggregateCommandFlag = 0;
    for (int i=1; i < argc; i++) {
        if (argv[i][0] == '-') {
            char *argument = strtok(argv[i], "=");
            if (strcmp(argument, "-size") == 0) {
                char *size_str = strtok(NULL, "=");
                if (size_str == NULL || !isInt(size_str)) {
                    fprintf(stderr, "Error: size=%s is not an int. in file: %s, on line: %d.\n", size_str, __FILE__ ,__LINE__);
                    exit(1);
                }
                char* endptr;
                output_size = strtol(size_str, &endptr, 10);
            } else if (strcmp(argument, "-prec") == 0) {
                char *prec_str = strtok(NULL, "=");
                if (prec_str == NULL || !isInt(prec_str)) {
                    fprintf(stderr, "Error: prec=%s is not an int. in file: %s, on line: %d.\n", prec_str, __FILE__ ,__LINE__);
                    exit(1);
                }
                char* endptr;
                output_prec = strtol(prec_str, &endptr, 10);
            } else {
                fprintf (stderr, "Error: %s is not a valid flag. in file: %s, on line: %d.\n", argument, __FILE__, __LINE__);
                exit(1);
            }
        } else {
            char *command = argv[i];
            strToUpper(command);    // Convert passed function name to uppercase
            if (commandFlag) {
                fprintf (stderr, "Error: A command was already passed before %s. in file: %s, on line: %d.\n", command, __FILE__, __LINE__);
                exit(1);
            }
            for (int j=0; j < LENGTH(validAggregateCommands); j++) {
                if (strcmp(command, validAggregateCommands[j]) == 0) {
                    commandFlag = 1;
                    aggregateCommandFlag = 1;
                    setFinalCommand(finalCommand, command, my_strlen(command));
                    break;
                }
            }
            if (strcmp(command, "PRINT") == 0) {
                commandFlag = 1;
                setFinalCommand(finalCommand, command, my_strlen(command));
                continue;
            }
            if (strcmp(command, "FILTER") == 0) {
                if ((i + 1) + 2 > argc) {
                    fprintf (stderr, "Error: Too few arguments for command %s. in file: %s, on line: %d.\n", command, __FILE__, __LINE__);
                    exit(1);
                }
                int validType = 0;
                char *type = argv[i+1];
                strToUpper(type);
                for (int j=0; j < filter_types_count; j++) {
                    if (strcmp(type, filter_types[j]) == 0) {
                        t = j;  // enum is just an int
                        validType = 1;
                        break;
                    }
                }
                if (!validType) {
                    fprintf (stderr, "Error: %s is not a valid type for the %s command. in file: %s, on line: %d.\n", type, command, __FILE__, __LINE__);
                    exit(1);
                }
                
                char *threshold_str = argv[i+2];
                if (!isDouble(threshold_str)) {
                    fprintf (stderr, "Error: %s is not double. in file: %s, on line: %d.\n", threshold_str, __FILE__, __LINE__);
                    exit(1);
                }
                sscanf(threshold_str, "%lf", &threshold);
                commandFlag = 1;
                i += 2;    // Skip the next 2 arguments as they were just evaluated
                setFinalCommand(finalCommand, command, my_strlen(command));
                continue;
            }
            if (strcmp(command, "SHIFT") == 0) {
                if ((i + 1) + 1 > argc) {
                    fprintf (stderr, "Error: Too few arguments for command %s. in file: %s, on line: %d.\n", command, __FILE__, __LINE__);
                    exit(1);
                }
                
                if (isDouble(argv[i+1])) {
                    sscanf(argv[i+1], "%lf", &shiftValue);
                } else {
                    fprintf (stderr, "Error: %s is not a double. in file: %s, on line: %d.\n", argv[i+1], __FILE__, __LINE__);
                    exit(1);
                }
                commandFlag = 1;
                i++;    // Skip the next argument as it was just evaluated
                setFinalCommand(finalCommand, command, my_strlen(command));
                continue;
            }
        }
    }

    if (!commandFlag) {
        fprintf (stderr, "Error: No command was passed\n");
        exit(1);
    }

    /* ----- Run the command ----- */

    // for (int i=0; i < row; i++) {
    //     for (int j=0; j < sizes[i]; j++) {
    //         printf("%.1f ", arr[i][j]);
    //     }
    //     printf("\n");
    // }

    if (aggregateCommandFlag) {
        for (int i=0; i < row; i++) {
            printf("%*.*f ", output_size, output_prec, aggregate(finalCommand, arr[i], sizes[i]));
        }
        printf("\n");
    } else if (strcmp(finalCommand, "PRINT") == 0) {
        for (int i=0; i < row; i++) {
            print(arr[i], sizes[i]);
        }
    } else if (strcmp(finalCommand, "FILTER") == 0) {
        for (int i=0; i < row; i++) {
            filter(arr[i], sizes[i], t, threshold);
        }
    } else if (strcmp(finalCommand, "SHIFT") == 0) {
        for (int i=0; i < row; i++) {
            shift(arr[i], sizes[i], shiftValue);
        }
    }

    /* ----- Free dynamic memory ----- */

    for (int i=0; i < row; i++) {
        free(arr[row]);
    }
    free(arr);
    free(finalCommand);
}