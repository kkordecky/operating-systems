#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include "signals.h"
#ifdef DYNAMIC
#include <dlfcn.h>
#endif

// Wysyła sobie SIGUSR1
void send_usr1() {
    printf("Wysyłam sygnał USR1\n");
    raise(SIGUSR1);
}

// ustawia reakcję na SIGUSR1
void usr2_handler(int sig, siginfo_t *info, void *ucontext) {
    int mode = info->si_value.sival_int;

    printf("Odebrano SIGUSR2 (%d)\n", mode);

#ifdef DYNAMIC
    // Załadowanie biblioteki
    void *handle = dlopen("./libsignals.so", RTLD_LAZY);
    if (!handle) {
        printf("dlopen error\n");
        return;
    }

    // Pobranie adresu wybranej funkcji 
    void (*f)();

    if (mode == 0) f = dlsym(handle, "sig_default");
    else if (mode == 1) f = dlsym(handle, "sig_ignore");
    else if (mode == 2) f = dlsym(handle, "sig_handle");
    else if (mode == 3) f = dlsym(handle, "sig_mask");

    if (!f) {
        printf("dlsym error\n");
        return;
    }

    // Wywołanie funkcji przez wskaźnik 
    f();

    // Zamknięcie biblioteki
    dlclose(handle);

#else
    if (mode == 0) sig_default();
    else if (mode == 1) sig_ignore();
    else if (mode == 2) sig_handle();
    else if (mode == 3) sig_mask();
#endif    
}

int main() {

    // ==== SETUP HANDLERA DLA SIGUSR2 =====
    struct sigaction act;
    act.sa_sigaction = usr2_handler; // nie blokuj innych sygnałów
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_SIGINFO;

    sigaction(SIGUSR2, &act, NULL);

    // =====================================

    printf("Child PID: %d\n", getpid());

    pause(); // pauza w oczekiwaniu na SIGUSR2
    // Pętla główna 1-20
    for (int i = 1; i <= 20; ++i) {
        printf("%d\n", i);

        // Wysłanie sygnału dla 5. i 15. sekundy
        if (i == 5 || i == 15) {
            send_usr1();
        }

        // Sprawdzenie pending przy i = 10
        if (i == 10) {
            sigset_t pending;
            sigpending(&pending); // pobranie oczekujcych sygnałów

            // Jeśli jest w środku SIGUSR1 -> usuń go z maski blokowanych
            if (sigismember(&pending, SIGUSR1)) {
                printf("Odblokowuję USR1\n");
            
            #ifdef DYNAMIC
            // === JW. w usr handlerze ===== 
                void *handle = dlopen("./libsignals.so", RTLD_LAZY);
                if (!handle) {
                    printf("dlopen error\n");
                    return;
                }

                void (*unblock)() = dlsym(handle, "sig_unblock");

                if (!unblock) {
                    printf("dlsym error\n");
                    return;
                }

                unblock();

                dlclose(handle);
            #else
                sig_unblock();
            #endif            
            }
        }

        sleep(1);
    }

    printf("Pętla została wykonana w całości\n");

    return 0;
}