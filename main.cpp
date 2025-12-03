#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


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

static Film films[MAX_FILMS];
static Hall halls[MAX_HALLS];
static Screening screenings[MAX_SCREENINGS];
static int film_count = 0;
static int hall_count = 0;
static int screening_count = 0;

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}


void menuVyberFilmu();
void menuZruseniRezervace();
void spravcovskeMenu();

void menuSpravaFilmu();
void menuSpravaSalu();
void menuSpravaVstupenek();


void loadFilms();
void loadHalls();
void loadScreenings();
void saveFilms();
void saveScreenings();


int findHallIndex(int hall_id);
const char *findFilmName(int film_id);
void zobrazSedadla(const Screening *s);

void hlavniMenu() {
    int choice;

    while (1) {
        clearScreen();
        printf("=========================================\n");
        printf("        REZERVACNI SYSTEM MULTIKINA\n");
        printf("=========================================\n");
        printf("1. Vyber filmu\n");
        printf("2. Zrusit rezervaci\n");
        printf("3. Spravcovske menu\n");
        printf("0. Konec programu\n");
        printf("-----------------------------------------\n");
        printf("Vyberte moznost: ");

        if (scanf_s("%d", &choice) != 1) {
            printf("Neplatny vstup!\n");
            return;
        }

        switch (choice) {
            case 1:
                menuVyberFilmu();
                break;
            case 2:
                menuZruseniRezervace();
                break;
            case 3:
                spravcovskeMenu();
                break;
            case 0:
                clearScreen();
                printf("Ukoncuji program...\n");
                return;
            default:
                printf("Neplatna volba! Stisknete enter...");
                getchar();
                getchar();
        }
    }
}


void spravcovskeMenu() {
    int choice;

    while (1) {
        clearScreen();
        printf("=========================================\n");
        printf("            SPRAVCOVSKE MENU\n");
        printf("=========================================\n");
        printf("1. Sprava filmu\n");
        printf("2. Sprava salu\n");
        printf("3. Sprava zakoupenych vstupenek\n");
        printf("0. Zpet do hlavniho menu\n");
        printf("-----------------------------------------\n");
        printf("Vyberte moznost: ");

        if (scanf_s("%d", &choice) != 1) {
            return;
        }

        switch (choice) {
            case 1:
                menuSpravaFilmu();
                break;
            case 2:
                menuSpravaSalu();
                break;
            case 3:
                menuSpravaVstupenek();
                break;
            case 0:
                return;
            default:
                printf("Neplatna volba! Stisknete enter...");
                getchar();
                getchar();
        }
    }
}

void menuVyberFilmu() {
    clearScreen();
    printf("Dostupne projekce:\n");
    for (int i = 0; i < screening_count; i++) {
        const Screening *s = &screenings[i];
        printf("ID %d | Film: %s | Sala %d | Cas: %s\n", s->id, findFilmName(s->film_id), s->hall_id, s->time);
    }

    if (screening_count == 0) {
        printf("Zadne projekce nejsou dostupne.\n");
        getchar();
        getchar();
        return;
    }

    printf("\nZadejte ID projekce pro rezervaci: ");
    int id;
    if (scanf_s("%d", &id) != 1) {
        return;
    }

    Screening *vybrana = NULL;
    for (int i = 0; i < screening_count; i++) {
        if (screenings[i].id == id) {
            vybrana = &screenings[i];
            break;
        }
    }

    if (!vybrana) {
        printf("Neplatne ID. Stisknete enter...\n");
        getchar();
        getchar();
        return;
    }

    zobrazSedadla(vybrana);
    printf("\nZadejte radek a sedadlo (1-based): ");
    int r, c;
    if (scanf_s("%d %d", &r, &c) != 2) {
        return;
    }

    if (r < 1 || r > vybrana->rows || c < 1 || c > vybrana->cols) {
        printf("Mimo rozsah salu. Stisknete enter...\n");
        getchar();
        getchar();
        return;
    }

    if (strcmp(vybrana->seats[r - 1][c - 1], "0") != 0) {
        printf("Misto je jiz obsazeno. Stisknete enter...\n");
        getchar();
        getchar();
        return;
    }

    char ticket[MAX_SEAT_LABEL];
    sprintf(ticket, "TICKET-%04X", rand() % 65536);
    strncpy(vybrana->seats[r - 1][c - 1], ticket, MAX_SEAT_LABEL - 1);
    vybrana->seats[r - 1][c - 1][MAX_SEAT_LABEL - 1] = '\0';

    saveScreenings();
    printf("Rezervace byla uspesna. Vase ID: %s\n", ticket);
    printf("Stisknete enter pro pokracovani...\n");
    getchar();
    getchar();
}

void menuZruseniRezervace() {
    clearScreen();
    char ticket[MAX_SEAT_LABEL];
    printf("Zadejte ID vstupenky k ruseni: ");
    scanf_s("%s", ticket, sizeof(ticket));

    for (int i = 0; i < screening_count; i++) {
        Screening *s = &screenings[i];
        for (int r = 0; r < s->rows; r++) {
            for (int c = 0; c < s->cols; c++) {
                if (strcmp(s->seats[r][c], ticket) == 0) {
                    strcpy(s->seats[r][c], "0");
                    saveScreenings();
                    printf("Listek zrusen. Stisknete enter...\n");
                    getchar();
                    getchar();
                    return;
                }
            }
        }
    }

    printf("Vstupenka s timto ID nebyla nalezena. Stisknete enter...\n");
    getchar();
    getchar();
}


void menuSpravaFilmu() {
    int choice;

    while (1) {
        clearScreen();
        printf("=========================================\n");
        printf("            SPRAVA FILMU\n");
        printf("=========================================\n");

        printf("Aktualni filmy:\n");
        for (int i = 0; i < film_count; i++) {
            printf("ID %d | %s\n", films[i].id, films[i].name);
        }

        printf("\n1. Pridat film\n");
        printf("2. Smazat film\n");
        printf("0. Zpet\n");
        printf("-----------------------------------------\n");
        printf("Vyberte moznost: ");

        if (scanf_s("%d", &choice) != 1) {
            return;
        }

        getchar(); 

        
        
        
        if (choice == 1) {
            char name[128];
            char id_input[32];
            int id;

            printf("Zadejte nazev filmu: ");
            fgets(name, sizeof(name), stdin);
            name[strcspn(name, "\n")] = '\0';

            printf("Zadejte ID (nechte prazdne pro automaticke prideleni): ");
            fgets(id_input, sizeof(id_input), stdin);

            if (id_input[0] == '\n') {
                
                id = 1;
                while (1) {
                    int used = 0;
                    for (int i = 0; i < film_count; i++) {
                        if (films[i].id == id) {
                            used = 1;
                            break;
                        }
                    }
                    if (!used)
                        break;
                    id++;
                }
            } else {
                id = atoi(id_input);
            }

            
            films[film_count].id = id;
            strncpy(films[film_count].name, name, sizeof(films[film_count].name) - 1);
            films[film_count].name[sizeof(films[film_count].name) - 1] = '\0';
            film_count++;

            saveFilms();
            printf("Film uspesne pridan.\n");
            getchar();
        }

        
        
        
        else if (choice == 2) {
            int id;
            printf("Zadejte ID filmu ke smazani: ");
            if (scanf_s("%d", &id) != 1) {
                getchar();
                continue;
            }

            int found = -1;
            for (int i = 0; i < film_count; i++) {
                if (films[i].id == id) {
                    found = i;
                    break;
                }
            }

            if (found == -1) {
                printf("Film s timto ID neexistuje.\n");
                getchar();
                getchar();
                continue;
            }

            
            for (int i = found; i < film_count - 1; i++) {
                films[i] = films[i + 1];
            }
            film_count--;

            saveFilms();
            printf("Film smazan.\n");
            getchar();
            getchar();
        }

        else if (choice == 0) {
            return;
        }

        else {
            printf("Neplatna volba! Stisknete enter...\n");
            getchar();
        }
    }
}


void menuSpravaSalu() {
    int choice;

    while (1) {
        clearScreen();
        printf("=========================================\n");
        printf("             SPRAVA SALU\n");
        printf("=========================================\n");
        printf("1. Pridat salu\n");
        printf("2. Smazat salu\n");
        printf("3. Zobrazit seznam salu\n");
        printf("0. Zpet\n");
        printf("-----------------------------------------\n");
        printf("Vyberte moznost: ");

        if (scanf_s("%d", &choice) != 1) {
            getchar();
            return;
        }
        getchar(); 

        
        
        
        if (choice == 3) {
            clearScreen();
            printf("Seznam existujicich salu:\n");
            for (int i = 0; i < hall_count; i++) {
                printf("ID %d | %dx%d\n", halls[i].id, halls[i].rows, halls[i].cols);
            }
            printf("\nStisknete enter pro navrat...");
            getchar();
            continue;
        }

        
        
        
        else if (choice == 1) {

            char id_input[32];
            int id;

            printf("Zadejte ID saly (nechte prazdne pro automaticke prideleni): ");
            fgets(id_input, sizeof(id_input), stdin);

            
            if (id_input[0] == '\n') {
                id = 1;
                while (1) {
                    int used = 0;
                    for (int i = 0; i < hall_count; i++) {
                        if (halls[i].id == id) {
                            used = 1;
                            break;
                        }
                    }
                    if (!used) break;
                    id++;
                }
            } else {
                id = atoi(id_input);
            }

            int rows, cols;
            printf("Zadejte pocet radku (1-%d): ", MAX_ROWS);
            scanf_s("%d", &rows); getchar();

            if (rows < 1 || rows > MAX_ROWS) {
                printf("Neplatny pocet radku!\n");
                getchar();
                continue;
            }

            printf("Zadejte pocet sloupcu (1-%d): ", MAX_COLS);
            scanf_s("%d", &cols); getchar();

            if (cols < 1 || cols > MAX_COLS) {
                printf("Neplatny pocet sloupcu!\n");
                getchar();
                continue;
            }

            
            halls[hall_count].id = id;
            halls[hall_count].rows = rows;
            halls[hall_count].cols = cols;

            
            for (int r = 0; r < rows; r++) {
                for (int c = 0; c < cols; c++) {
                    halls[hall_count].seats[r][c] = '0';
                }
            }

            
            generateSectorsForHall(&halls[hall_count]);

            hall_count++;

            
            FILE *f = fopen("halls.db", "w");
            if (f) {
                for (int i = 0; i < hall_count; i++) {
                    fprintf(f, "%d;%d;%d\n", halls[i].id, halls[i].rows, halls[i].cols);
                    for (int r = 0; r < halls[i].rows; r++) {
                        for (int c = 0; c < halls[i].cols; c++) {
                            fputc(halls[i].seats[r][c], f);
                        }
                        fputc('\n', f);
                    }
                }
                fclose(f);
            }

            printf("Sala byla uspesne pridana.\n");
            getchar();
            continue;
        }

        
        
        
        else if (choice == 2) {
            int id;
            printf("Zadejte ID saly ke smazani: ");
            scanf_s("%d", &id); getchar();

            int index = -1;
            for (int i = 0; i < hall_count; i++) {
                if (halls[i].id == id) {
                    index = i;
                    break;
                }
            }

            if (index == -1) {
                printf("Sala s timto ID neexistuje.\n");
                getchar();
                continue;
            }

            
            int linked = 0;
            for (int i = 0; i < screening_count; i++) {
                if (screenings[i].hall_id == id) {
                    linked = 1;
                    break;
                }
            }

            if (linked) {
                printf("Tuto salu nelze smazat, existuji projekce, ktere ji pouzivaji!\n");
                getchar();
                continue;
            }

            
            for (int i = index; i < hall_count - 1; i++) {
                halls[i] = halls[i + 1];
            }
            hall_count--;

            
            FILE *f = fopen("halls.db", "w");
            if (f) {
                for (int i = 0; i < hall_count; i++) {
                    fprintf(f, "%d;%d;%d\n", halls[i].id, halls[i].rows, halls[i].cols);
                    for (int r = 0; r < halls[i].rows; r++) {
                        for (int c = 0; c < halls[i].cols; c++) {
                            fputc(halls[i].seats[r][c], f);
                        }
                        fputc('\n', f);
                    }
                }
                fclose(f);
            }

            printf("Sala byla smazana.\n");
            getchar();
            continue;
        }

        
        else if (choice == 0) {
            return;
        }

        else {
            printf("Neplatna volba! Stisknete enter...");
            getchar();
        }
    }
}

void menuSpravaVstupenek() {
    clearScreen();
    printf("Prehled prodeju:\n");
    for (int i = 0; i < screening_count; i++) {
        int sold = 0;
        for (int r = 0; r < screenings[i].rows; r++) {
            for (int c = 0; c < screenings[i].cols; c++) {
                if (strcmp(screenings[i].seats[r][c], "0") != 0) {
                    sold++;
                }
            }
        }
        printf("Projekce %d (%s) - prodano %d/%d mist\n", screenings[i].id, findFilmName(screenings[i].film_id), sold, screenings[i].rows * screenings[i].cols);
    }
    printf("Stisknete enter pro navrat...\n");
    getchar();
    getchar();
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
        if (sscanf_s(line, "%d;%d;%d", &id, &rows, &cols) != 3) {
            continue;
        }
        halls[hall_count].id = id;
        halls[hall_count].rows = rows;
        halls[hall_count].cols = cols;
        for (int r = 0; r < rows; r++) {
            if (fgets(line, sizeof(line), f) == NULL) {
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

int main() {
    srand((unsigned) time(NULL));
    loadFilms();
    loadHalls();
    loadScreenings();
    hlavniMenu();
    return 0;
}

