#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "storage.h"

void loadFilms() {
    film_count = 0;
    FILE *f = fopen("films.db", "r");
    if (!f) {
        return;
    }
    char line[256];
    while (fgets(line, sizeof(line), f) && film_count < MAX_FILMS) {
        line[strcspn(line, "\n")] = '\0';
        if (strlen(line) == 0) {
            continue;
        }
        char *semi = strchr(line, ';');
        if (!semi) {
            continue;
        }
        *semi = '\0';
        films[film_count].id = atoi(line);
        strncpy(films[film_count].name, semi + 1, sizeof(films[film_count].name) - 1);
        films[film_count].name[sizeof(films[film_count].name) - 1] = '\0';
        film_count++;
    }
    fclose(f);
}

void loadHalls() {
    hall_count = 0;
    FILE *f = fopen("halls.db", "r");
    if (!f) {
        return;
    }
    char line[256];
    while (fgets(line, sizeof(line), f) && hall_count < MAX_HALLS) {
        if (strlen(line) < 3) {
            continue;
        }
        int id, rows, cols;
        char name[128];

        
        if (sscanf_s(line, "%d;%d;%d;%127[^\n]", &id, &rows, &cols, name, sizeof(name)) != 4) {
            continue;
        }
        halls[hall_count].id = id;
        halls[hall_count].rows = rows;
        halls[hall_count].cols = cols;
        strncpy(halls[hall_count].name, name, sizeof(halls[hall_count].name));
        halls[hall_count].name[sizeof(halls[hall_count].name) - 1] = '\0';

        for (int r = 0; r < rows; r++) {
            if (!fgets(line, sizeof(line), f)) {
                break;
            }
            line[strcspn(line, "\n")] = '\0';
            for (int c = 0; c < cols && line[c]; c++) {
                halls[hall_count].seats[r][c] = line[c];
            }
        }
        hall_count++;
    }
    fclose(f);
}

void loadScreenings() {
    screening_count = 0;
    FILE *f = fopen("screenings.db", "r");
    if (!f) {
        return;
    }
    char line[512];
    while (fgets(line, sizeof(line), f) && screening_count < MAX_SCREENINGS) {
        if (strlen(line) < 3) {
            continue;
        }
        Screening *s = &screenings[screening_count];
        line[strcspn(line, "\n")] = '\0';

        char *token = strtok(line, ";");
        if (!token) {
            continue;
        }
        s->id = atoi(token);
        token = strtok(NULL, ";");
        if (!token) {
            continue;
        }
        s->film_id = atoi(token);
        token = strtok(NULL, ";");
        if (!token) {
            continue;
        }
        s->hall_id = atoi(token);
        token = strtok(NULL, "");
        if (!token) {
            continue;
        }
        strncpy(s->time, token, sizeof(s->time) - 1);
        s->time[sizeof(s->time) - 1] = '\0';

        if (!fgets(line, sizeof(line), f)) {
            break;
        }
        int rows, cols;
        if (sscanf_s(line, "%d;%d", &rows, &cols) != 2) {
            break;
        }
        s->rows = rows;
        s->cols = cols;

        for (int r = 0; r < rows; r++) {
            if (!fgets(line, sizeof(line), f)) {
                break;
            }
            line[strcspn(line, "\n")] = '\0';
            int c = 0;
            char *item = strtok(line, ",");
            while (item && c < cols) {
                strncpy(s->seats[r][c], item, MAX_SEAT_LABEL - 1);
                s->seats[r][c][MAX_SEAT_LABEL - 1] = '\0';
                item = strtok(NULL, ",");
                c++;
            }
            while (c < cols) {
                strcpy(s->seats[r][c], "0");
                c++;
            }
        }
        screening_count++;
    }
    fclose(f);
}

void saveFilms() {
    FILE *f = fopen("films.db", "w");
    if (!f) {
        return;
    }
    for (int i = 0; i < film_count; i++) {
        fprintf(f, "%d;%s\n", films[i].id, films[i].name);
    }
    fclose(f);
}

void saveScreenings() {
    FILE *f = fopen("screenings.db", "w");
    if (!f) {
        return;
    }
    for (int i = 0; i < screening_count; i++) {
        Screening *s = &screenings[i];
        fprintf(f, "%d;%d;%d;%s\n", s->id, s->film_id, s->hall_id, s->time);
        fprintf(f, "%d;%d\n", s->rows, s->cols);
        for (int r = 0; r < s->rows; r++) {
            for (int c = 0; c < s->cols; c++) {
                fprintf(f, "%s", s->seats[r][c]);
                if (c < s->cols - 1) {
                    fprintf(f, ",");
                }
            }
            fprintf(f, "\n");
        }
    }
    fclose(f);
}
