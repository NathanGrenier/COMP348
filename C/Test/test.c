#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char* testFunc(char* str) {
    str[0] = 'E';
    return str;
}

int main() {
    
    char *str = "This is a string";
    char str1[] = "Another String";

    printf("%s\n", testFunc(str1));

    return 0;
}
