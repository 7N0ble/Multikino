#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menus.h"
#include "utils.h"
#include "storage.h"
#include "qr.h"


static void generateSectorsForHall(Hall *hall) {
    for (int r = 0; r < hall->rows; r++) {
        for (int c = 0; c < hall->cols; c++) {
            hall->sector[r][c] = 0; 
        }
    }
}

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
        printf("4. Sprava projekci\n");
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
            case 4:
                menuSpravaProjekci();
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

void menuSpravaProjekci() {
    int choice;

    while (1) {
        clearScreen();
        printf("=========================================\n");
        printf("            SPRAVA PROJEKCI\n");
        printf("=========================================\n");
        printf("1. Pridat projekci\n");
        printf("2. Smazat projekci\n");
        printf("3. Upravit projekci\n");
        printf("4. Zobrazit seznam projekci\n");
        printf("5. Otevrit projekci (zobrazit salu)\n");
        printf("0. Zpet\n");
        printf("-----------------------------------------\n");
        printf("Vyberte moznost: ");

        if (scanf_s("%d", &choice) != 1) {
            getchar();
            return;
        }
        getchar();

        
        
        
        if (choice == 4) {
            clearScreen();
            printf("Seznam projekci:\n\n");

            for (int i = 0; i < screening_count; i++) {
                Screening *s = &screenings[i];
                printf("ID %d | Film: %s | Sala %d | Cas: %s\n",
                       s->id, findFilmName(s->film_id), s->hall_id, s->time);
            }

            printf("\nStisknete enter...");
            getchar();
            continue;
        }

        
        
        
        else if (choice == 1) {

            int id;
            char id_input[32];
            printf("Zadejte ID projekce (nechte prazdne pro auto prideleni): ");
            fgets(id_input, sizeof(id_input), stdin);

            
            if (id_input[0] == '\n') {
                id = 1;
                while (1) {
                    int used = 0;
                    for (int i = 0; i < screening_count; i++) {
                        if (screenings[i].id == id) { used = 1; break; }
                    }
                    if (!used) break;
                    id++;
                }
            } else {
                id = atoi(id_input);
                for (int i = 0; i < screening_count; i++) {
                    if (screenings[i].id == id) {
                        printf("Chyba: projekce s ID %d jiz existuje!\n", id);
                        getchar();
                        return;
                    }
                }
            }

            
            printf("Dostupne filmy:\n");
            for (int i = 0; i < film_count; i++) {
                printf("ID %d | %s\n", films[i].id, films[i].name);
            }

            int film_id;
            printf("Zadejte ID filmu: ");
            scanf_s("%d", &film_id); getchar();

            int film_found = 0;
            for (int i = 0; i < film_count; i++) {
                if (films[i].id == film_id) { film_found = 1; break; }
            }
            if (!film_found) {
                printf("Tento film neexistuje!\n");
                getchar();
                continue;
            }

            
            printf("Dostupne sály:\n");
            for (int i = 0; i < hall_count; i++) {
                printf("ID %d | %dx%d | %s\n", halls[i].id, halls[i].rows, halls[i].cols, halls[i].name);
            }

            int hall_id;
            printf("Zadejte ID saly: ");
            scanf_s("%d", &hall_id); getchar();

            int hall_index = findHallIndex(hall_id);
            if (hall_index == -1) {
                printf("Tato sala neexistuje!\n");
                getchar();
                continue;
            }

            
            char time[32];
            printf("Zadejte cas projekce (napr. 18:30): ");
            fgets(time, sizeof(time), stdin);
            time[strcspn(time, "\n")] = '\0';

            Screening *s = &screenings[screening_count];
            s->id = id;
            s->film_id = film_id;
            s->hall_id = hall_id;
            strncpy(s->time, time, sizeof(s->time));

            
            s->rows = halls[hall_index].rows;
            s->cols = halls[hall_index].cols;

            
            for (int r = 0; r < s->rows; r++)
                for (int c = 0; c < s->cols; c++)
                    strcpy(s->seats[r][c], "0");

            screening_count++;
            saveScreenings();

            printf("Projekce byla uspesne pridana.\n");
            getchar();
            continue;
        }

        
        
        
        else if (choice == 2) {
            int id;
            printf("Zadejte ID projekce ke smazani: ");
            scanf_s("%d", &id); getchar();

            int index = -1;
            for (int i = 0; i < screening_count; i++) {
                if (screenings[i].id == id) { index = i; break; }
            }

            if (index == -1) {
                printf("Projekce neexistuje.\n");
                getchar();
                continue;
            }

            
            for (int i = index; i < screening_count - 1; i++)
                screenings[i] = screenings[i + 1];

            screening_count--;
            saveScreenings();

            printf("Projekce smazana.\n");
            getchar();
            continue;
        }

        
        
        
        else if (choice == 3) {
            int id;
            printf("Zadejte ID projekce k uprave: ");
            scanf_s("%d", &id); getchar();

            Screening *s = NULL;
            for (int i = 0; i < screening_count; i++) {
                if (screenings[i].id == id) { s = &screenings[i]; break; }
            }

            if (!s) {
                printf("Projekce nenalezena.\n");
                getchar();
                continue;
            }

            printf("Uprava projekce %d:\n", s->id);
            printf("1. Zmenit film\n");
            printf("2. Zmenit salu\n");
            printf("3. Zmenit cas\n");
            printf("0. Zpet\n");
            printf("Vyber: ");

            int ch;
            scanf_s("%d", &ch); getchar();

            if (ch == 1) {
                printf("Filmy:\n");
                for (int i = 0; i < film_count; i++)
                    printf("ID %d | %s\n", films[i].id, films[i].name);

                printf("Novy film ID: ");
                int fid;
                scanf_s("%d", &fid); getchar();
                s->film_id = fid;
            }
            else if (ch == 2) {
                printf("Saly:\n");
                for (int i = 0; i < hall_count; i++)
                    printf("ID %d | %dx%d | %s\n", halls[i].id,
                           halls[i].rows, halls[i].cols, halls[i].name);

                printf("Nova sala ID: ");
                int hid;
                scanf_s("%d", &hid); getchar();

                int idx = findHallIndex(hid);
                if (idx == -1) {
                    printf("Sala neexistuje!\n"); getchar(); continue;
                }

                s->hall_id = hid;
                s->rows = halls[idx].rows;
                s->cols = halls[idx].cols;

                
                for (int r = 0; r < s->rows; r++)
                    for (int c = 0; c < s->cols; c++)
                        strcpy(s->seats[r][c], "0");
            }
            else if (ch == 3) {
                printf("Novy cas: ");
                fgets(s->time, sizeof(s->time), stdin);
                s->time[strcspn(s->time, "\n")] = '\0';
            }

            saveScreenings();
            printf("Projekce upravena.\n");
            getchar();
            continue;
        }

        
        
        
        else if (choice == 5) {
            int id;
            printf("Zadejte ID projekce: ");
            scanf_s("%d", &id); getchar();

            Screening *s = NULL;
            for (int i = 0; i < screening_count; i++) {
                if (screenings[i].id == id) { s = &screenings[i]; break; }
            }

            if (!s) {
                printf("Projekce nenalezena.\n"); getchar(); continue;
            }

            clearScreen();
            printf("Projekce %d | Film: %s | Sala %d | Cas: %s\n",
                   s->id, findFilmName(s->film_id), s->hall_id, s->time);

            zobrazSedadla(s);

            printf("\nStisknete enter...");
            getchar();
            continue;
        }

        else if (choice == 0)
            return;

        else {
            printf("Neplatna volba!\n"); getchar();
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
    printf("\nZadejte radek a sedadlo (cislo medzera cislo): ");
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

    
    int hall_index = findHallIndex(vybrana->hall_id);
    (void)hall_index;

    
    int randomCode = rand() % 65536;

    
    snprintf(ticket, sizeof(ticket),
             "TICKET-H%d-R%d-C%d-%04X",
             vybrana->hall_id,
             r, c,
             randomCode);

    
    strncpy(vybrana->seats[r - 1][c - 1], ticket, MAX_SEAT_LABEL - 1);
    vybrana->seats[r - 1][c - 1][MAX_SEAT_LABEL - 1] = '\0';

    saveScreenings();
    printf("Rezervace byla uspesna. Vase ID: %s\n", ticket);
    printf("SVOJE ID SI ULOZTE, REKLAMACE BEZ ID LISTKA NEAKCEPTUJEME!!\n", ticket);


    
    
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
                    printf("Lístek zrusen. Stisknete enter...\n");
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
            printf("Film uspesne pridan. Stisknete enter...\n");
            getchar();
        }

        else if (choice == 2) {
            int id;
            printf("Zadejte ID filmu ke smazani: ");
            if (scanf_s("%d", &id) != 1) {
                getchar();
                continue;
            }
            getchar();

            int found = -1;
            for (int i = 0; i < film_count; i++) {
                if (films[i].id == id) {
                    found = i;
                    break;
                }
            }
            if (found == -1) {
                printf("Film s timto ID neexistuje. Stisknete enter...\n");
                getchar();
                continue;
            }

            
            for (int i = 0; i < screening_count; i++) {
                if (screenings[i].film_id == id) {
                    printf("Nelze smazat film, existuji projekce, ktere ho pouzivaji!\n");
                    getchar();
                    found = -1;
                    break;
                }
            }
            if (found == -1) {
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
                printf("ID %d | %s | %dx%d\n", halls[i].id, halls[i].name, halls[i].rows, halls[i].cols);
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

                
                for (int i = 0; i < hall_count; i++) {
                    if (halls[i].id == id) {
                        printf("Chyba: Sala s ID %d jiz existuje!\n", id);
                        printf("Stisknete enter...");
                        getchar();
                        return;
                    }
                }
            }

            
            char name[128];
            printf("Zadejte nazev saly: ");
            fgets(name, sizeof(name), stdin);
            name[strcspn(name, "\n")] = '\0';

            
            for (int i = 0; i < hall_count; i++) {
                if (strcmp(halls[i].name, name) == 0) {
                    printf("Chyba: Sala s nazvem '%s' jiz existuje!\n", name);
                    printf("Stisknete enter...");
                    getchar();
                    return;
                }
            }

            int rows, cols;

            printf("Zadejte pocet radku (1-%d): ", MAX_ROWS);
            scanf_s("%d", &rows); getchar();
            if (rows < 1 || rows > MAX_ROWS) {
                printf("Neplatny pocet radku!\n");
                getchar();
                return;
            }

            printf("Zadejte pocet sloupcu (1-%d): ", MAX_COLS);
            scanf_s("%d", &cols); getchar();
            if (cols < 1 || cols > MAX_COLS) {
                printf("Neplatny pocet sloupcu!\n");
                getchar();
                return;
            }

            
            halls[hall_count].id = id;
            halls[hall_count].rows = rows;
            halls[hall_count].cols = cols;

            strncpy(halls[hall_count].name, name, sizeof(halls[hall_count].name) - 1);
            halls[hall_count].name[sizeof(halls[hall_count].name) - 1] = '\0';

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

                    
                    fprintf(f, "%d;%d;%d;%s\n", halls[i].id, halls[i].rows, halls[i].cols, halls[i].name);

                    for (int r = 0; r < halls[i].rows; r++) {
                        for (int c = 0; c < halls[i].cols; c++) {
                            fputc(halls[i].seats[r][c], f);
                        }
                        fputc('\n', f);
                    }
                }
                fclose(f);
            }

            printf("Sala '%s' byla uspesne pridana.\n", name);
            getchar();
            return;
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
                    
                    fprintf(f, "%d;%d;%d;%s\n", halls[i].id, halls[i].rows, halls[i].cols, halls[i].name);
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
