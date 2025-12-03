#ifndef DATA_H
#define DATA_H

#include <stdio.h>


#define MAX_FILMS 128
#define MAX_HALLS 32
#define MAX_SCREENINGS 256
#define MAX_ROWS 50
#define MAX_COLS 50
#define MAX_SEAT_LABEL 32

typedef struct {
    int id;
    char name[128];
} Film;

typedef struct {
    int id;
    int rows;
    int cols;
    char name[128];
    char seats[MAX_ROWS][MAX_COLS];
    int sector[MAX_ROWS][MAX_COLS];
} Hall;

typedef struct {
    int id;
    int film_id;
    int hall_id;
    char time[32];
    int rows;
    int cols;
    char seats[MAX_ROWS][MAX_COLS][MAX_SEAT_LABEL];
} Screening;


extern Film films[MAX_FILMS];
extern Hall halls[MAX_HALLS];
extern Screening screenings[MAX_SCREENINGS];
extern int film_count;
extern int hall_count;
extern int screening_count;

#endif 
