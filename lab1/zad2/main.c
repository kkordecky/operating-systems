#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Błąd: należy podać dokładnie dwa argumenty (N i M)\n");
        fprintf(stderr, "Użycie: %s N M\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);
    int M = atoi(argv[2]);

    if (N <= 0) {
        fprintf(stderr, "Błąd: N musi być liczbą dodatnią\n");
        return 1;
    }

    if (M <= 0) {
        fprintf(stderr, "Błąd: M musi być liczbą dodatnią\n");
        return 1;
    }

    for (int i = 0; i < N; ++i) {
        if (fork() == 0) {
            char m[11];
            sprintf(m, "%d", M);
            execl("./child", "child", m, NULL);
        }
    }

    for (int i = 0; i < N; ++i) {
        wait(NULL);
    }

    printf("Rodzic  (PID: %d)\n", getpid());

    return 0;
}