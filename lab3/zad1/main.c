#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include "integral.h"

// Pomiar czasu
double get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

int main(int argc, char *argv[]) {

    // ==== Weryfikacja argumentów ===  
    if (argc != 3) {
        printf("Użycie: %s <dx> <n>\n", argv[0]);
        return 1;
    }
    // ===========================

    double dx = atof(argv[1]);
    int n = atoi(argv[2]);

    for (int k = 1; k <= n; k++) {

        double start = get_time(); // zaczynam pomiar czasu

        int pipes[k][2];
        pid_t pids[k];

        // Tworzenie procesów potomnych 
        for (int i = 0; i < k; i++) {

            if (pipe(pipes[i]) == -1) {
                perror("pipe");
                exit(1);
            }

            pids[i] = fork();
            
            // ===== OPERACJE DZIECKA ===== 
            if (pids[i] == 0) {

                close(pipes[i][0]); // zamykamy odczyt

                double a = (double)i / k;
                double b = (double)(i + 1) / k;

                double result = compute(a, b, dx);

                write(pipes[i][1], &result, sizeof(double));
                close(pipes[i][1]);

                exit(0);
            }
            // ==== END: OPERACJE DZIECKA ===== 
        }

        // ==== OPERACJE RODZICA ====  
        double total = 0.0;

        for (int i = 0; i < k; i++) {
            close(pipes[i][1]); // zamykamy zapis

            double partial;
            read(pipes[i][0], &partial, sizeof(double));
            close(pipes[i][0]);

            total += partial;
        }

        // oczekiwanie na wszystkie dzieci 
        for (int i = 0; i < k; i++) {
            wait(NULL);
        }

        // Koniec pomiaru
        double end = get_time();  

        printf("k = %d | wynik = %.10f | czas = %.5f s\n",
               k, total, end - start);
    }

    return 0;
}