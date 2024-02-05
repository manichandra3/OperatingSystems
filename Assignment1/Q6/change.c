#include "stdio.h"
#include "stdlib.h"
int main() {
    char ch;
    printf("Opposite Case: ");

    while ((ch = getchar()) != '\0') {

        if ((65 <= ch && ch <= 90)) {
            putchar(ch + ('a' - 'A'));
        } else if (97 <= ch && ch <= 122) {
            putchar(ch - ('a' - 'A'));
        } else {
            putchar(ch);
        }

    }
    return 0;
}