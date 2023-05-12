#include <stdio.h>
#include <stdlib.h>

void create_file() {
    char* filename="100MB-File.txt";
    FILE* fp;
    long int i;    // Check if file already exists
    
    if (fopen(filename, "r") != NULL) {
        printf("File already exists\n");
        return;
    }


    fp = fopen(filename, "wb");
    if (fp == NULL) {
        printf("Error opening file\n");
        return;
    }

    // //100MB
    for (i = 0; i < 10000000L; i++) { 
        fputc('a', fp);
    }

    //100kB
    // for (i = 0; i < 10000L; i++) { 
    //     fputc('a', fp);
    // }
    fputc('x', fp);
    fclose(fp);
    printf("File created successfully\n");
}


// int main() {
//     create_file();
//     return 0;
// }