#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include "child.h"

int main(int argc, char *argv[]) {
    // Weryfikacja argumentów
    if (argc != 2) {
        printf("Wywołanie: %s default|mask|ignore|handle\n", argv[0]);
        return 1;
    }

    // Ustawienie właściwego sygnału na podstawie argumentu
    if (strcmp(argv[1], "default") == 0) {
        sig_default();
    } else if (strcmp(argv[1], "mask") == 0) {
        sig_mask();
    } else if (strcmp(argv[1], "ignore") == 0) {
        sig_ignore();
    } else if (strcmp(argv[1], "handle") == 0) {
        sig_handle();
    } else {
        printf("Niepoprawny argument!\n");
        return 1;
    }

    // Pętla główna programu (1-20)
    for (int i = 1; i <= 20; ++i) {
        printf("%d\n", i);


        // Wysłanie sygnału dla 5. i 15. sekundy
        if (i == 5 || i == 15) {
            send_usr1(); 
        }

        // Sprawdzenie pending przy i = 10
        if (i == 10) {
            sigset_t pending;
            sigpending(&pending); // Pobranie oczekujących sygnałów

            // Jeśli jest w środku SIGUSR1 -> usuń go z maski blokowanych 
            if (sigismember(&pending, SIGUSR1)) {
                printf("Odblokowuję USR1\n");
                sig_unblock();
            }
        }

        sleep(1);
    }

    printf("Pętla została wykonana w całości\n");

    return 0;
}