#include <stdio.h>
#include <stdlib.h>

void create_file() {
    char* filename="100MB-File.txt";
    FILE* fp;
    long int i;

    fp = fopen(filename, "wb");
    if (fp == NULL) {
        printf("Error opening file\n");
        return;
    }

    //100MB
    for (i = 0; i < 10000000L; i++) { 
        fputc('a', fp);
    }

    //100kB
    // for (i = 0; i < 10000L; i++) { 
    //     fputc('a', fp);
    // }

    fclose(fp);
    printf("File created successfully\n");
}


// int main() {
//     create_file();
//     return 0;
// }