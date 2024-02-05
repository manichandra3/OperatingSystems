#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <fcntl.h>
// second
int main() {
    if(open("input.txt",O_RDWR)<0){
        fprintf(stderr,"File not found\n");
    }
    else {
        FILE *file = fopen("input.txt", "r");

        int ch;
        int nonAlphabeticCount = 0;

        while ((ch = fgetc(file)) != EOF) {
            if (!isalpha(ch)) {
                nonAlphabeticCount++;
            }

            putchar(ch);
        }

        fclose(file);

        fprintf(stdout, "\nNon-alphabetic count: %d\n", nonAlphabeticCount);
    }

    exit(0);
}
