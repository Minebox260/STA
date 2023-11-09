#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define NB 6

int main() {
    int i = 0;
    clock_t start;
    clock_t end;

    while (i < NB) {
        start = clock();
        end = start;
        while ((float)(end-start)/(float)CLOCKS_PER_SEC * 1000 < 500) end = clock();
        printf("Durée de la boucle : %fs\n", (float)(end - start)/CLOCKS_PER_SEC);
        i++;
    }
    i = 0;
    printf("\n\n");
    while (i < NB) {
        start = clock();
        usleep(500000);
        end = clock();
        printf("Durée de la boucle : %fs\n", (float)(end - start)/CLOCKS_PER_SEC);
        i++;
    }
}