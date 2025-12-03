#ifndef UTILS_H
#define UTILS_H

#include "data.h"

void clearScreen();
int findHallIndex(int hall_id);
const char *findFilmName(int film_id);
void zobrazSedadla(const Screening *s);

#endif 
