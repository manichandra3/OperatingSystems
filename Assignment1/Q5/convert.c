//
// Created by Mani Chandra Ganapathri on 22/01/24.
//
#include "stdio.h"
int main() {
    char ch;
    while ((ch = getchar()) != EOF) {

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