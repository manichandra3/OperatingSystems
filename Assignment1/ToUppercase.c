#include<stdio.h>
#include<ctype.h>
// third
int main() {
    FILE *file = fopen("input.txt", "r+");

    int ch;
    int convCount = 0;
    int converted[100];
    int i = 0;

    while ((ch = fgetc(file)) != EOF) {
        if (isalpha(ch)) {
            if (islower(ch)) {
                ch = toupper(ch);
                convCount++;
                converted[i] = 1;
            } else if (isupper(ch)) {
                ch = tolower(ch);
                convCount++;
                converted[i] = -1;
            }
            fseek(file, -1, SEEK_CUR);
            fputc(ch, file);
            fseek(file, 0, SEEK_CUR);
        }
        i++;
    }

    fclose(file);

    printf("\nConversion count: %d\n", convCount);//stdout
    printf("Conversion array: ");//stdout
    for (int j = 0; j < i; j++) {
        printf("%d ", converted[j]);
    }
    printf("\n");

    return 0;
}
