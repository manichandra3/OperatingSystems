#include "stdio.h"

int main() {
    char ch;
    int count = 0;
    while ((ch = getchar()) != EOF) {

        if (!((65 <= ch && ch <= 90) || (97 <= ch && ch <= 122))) {
            count++;
            printf("%c", ch);
        }
    }
    printf("The number of non-alphabetic characters are: %d\n", count);


    return 0;
}