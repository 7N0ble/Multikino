#include <ctime>
#include <cstdlib>
#include "storage.h"
#include "menus.h"

int main() {
    srand((unsigned) time(nullptr));
    loadFilms();
    loadHalls();
    loadScreenings();
    hlavniMenu();
    return 0;
}
