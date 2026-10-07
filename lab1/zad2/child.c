#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
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
    
    for (int i = 0; i < M; ++i) {
        printf("Potomek (PID: %d)\n", getpid());
        sleep(0.25);
    }

    return 0;
}