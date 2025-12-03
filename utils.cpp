#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "utils.h"

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int findHallIndex(int hall_id) {
    for (int i = 0; i < hall_count; i++) {
        if (halls[i].id == hall_id) {
            return i;
        }
    }
    return -1;
}

const char *findFilmName(int film_id) {
    for (int i = 0; i < film_count; i++) {
        if (films[i].id == film_id) {
            return films[i].name;
        }
    }
    return "Neznamy film";
}

void zobrazSedadla(const Screening *s) {
    printf("\nMapa salu (%dx%d):\n", s->rows, s->cols);
    printf("   ");
    for (int c = 0; c < s->cols; c++) {
        printf("%2d ", c + 1);
    }
    printf("\n");
    for (int r = 0; r < s->rows; r++) {
        printf("%2d ", r + 1);
        for (int c = 0; c < s->cols; c++) {
            if (strcmp(s->seats[r][c], "0") == 0) {
                printf("[ ]");
            } else {
                printf("[X]");
            }
        }
        printf("\n");
    }
}
