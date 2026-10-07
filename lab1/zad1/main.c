#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define M 5

int zmiennaGlobalna = 0;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Błąd: należy podać dokładnie jeden argument (N)\n");
        fprintf(stderr, "Użycie: %s N\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);
    if (N <= 0) {
        fprintf(stderr, "Błąd: N musi być liczbą dodatnią\n");
        return 1;
    }

    for (int i = 0; i < N; ++i) {
        // pid_t pid = fork();
        pid_t pid = vfork();

        if (pid == 0) {
            zmiennaGlobalna++;

            for (int j = 0; j < M; ++j) {
                printf("Potomek (PID: %d)\n", getpid());
                sleep(0.25);
            }

            exit(0);
        }
    }

    for (int i = 0; i < N; ++i) {
        wait(NULL);
    }

    printf("Rodzic (PID: %d) zmiennaGlobalna = %d\n", getpid(), zmiennaGlobalna);

    return 0;
}