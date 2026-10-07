#include "definitions.h"

int main(int argc, char *argv[]) {
    // Obsługa potenncjalnie występujących błędów
    if (argc != 2) {
        fprintf(stderr, "Błąd: należy podać dokładnie jeden argument (M)\n");
        fprintf(stderr, "Użycie: %s M\n", argv[0]);
        return 1;
    }

    int M = atoi(argv[1]);
    if (M <= 0) {
        fprintf(stderr, "Błąd: M musi być liczbą dodatnią\n");
        return 1;
    }

    FILE* fptr; // Wskaźnik do pliku 

    // Otwieram plik w trybie append
    fptr = fopen(OUTPUT_FILE, "a");

    // Sprawdzam, czy plik został otworzony z sukcesem
    if (fptr == NULL) {
        fprintf(stderr, "Błąd: Plik nie został otworzony\n");
    }

    
    // Pętla zapisująca do pliku  
    for (int i = 0; i < M; ++i) {
        fprintf(fptr, "Potomek (PID: %d)\n", getpid());
        fflush(fptr); 
        sleep(0.25);
    }

    fclose(fptr);
    return 0;
}