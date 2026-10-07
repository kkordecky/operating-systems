#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void handler(int sig) {
    printf("Wywołano handler dla sygnału %d\n", sig);
}

// ==== FUNCKJE USTAWIAJĄCE REAKCJĘ NA SIGUSR1 ====
void sig_default() {
    printf("Wywołano funkcję 'sig_default()'\n");
    signal(SIGUSR1, SIG_DFL);
}

void sig_ignore() {
    printf("Wywołano funkcję 'sig_ignore()'\n");
    signal(SIGUSR1, SIG_IGN);
}

void sig_handle() {
    printf("Wywołano funkcję 'sig_handle()'\n");
    signal(SIGUSR1, handler);
}

void sig_mask() {
    printf("Wywołano funkcję 'sig_mask()'\n");

    sigset_t set;
    sigemptyset(&set); // init pustego zbioru sygnałów maskowanych 
    sigaddset(&set, SIGUSR1); // Dodanie do zbioru SIGUSR1
    sigprocmask(SIG_BLOCK, &set, NULL); // Zablokowanie sygnałów ze zbioru
}


void sig_unblock() {
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1); // jw. 

    sigprocmask(SIG_UNBLOCK, &set, NULL); //  Odblokowanie sygnałów ze zbioru
}
// ==============

// Wysyła sobie SIGUSR1
void send_usr1() {
    printf("Wysyłam sygnał USR1\n");
    raise(SIGUSR1);
}

// ustawia reakcję na SIGUSR1
void usr2_handler(int sig, siginfo_t *info, void *ucontext) {
    int mode = info->si_value.sival_int;

    printf("Odebrano SIGUSR2 (%d)\n", mode);

    if (mode == 0) sig_default();
    else if (mode == 1) sig_ignore();
    else if (mode == 2) sig_handle();
    else if (mode == 3) sig_mask();

    
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
                sig_unblock();
            }
        }

        sleep(1);
    }

    printf("Pętla została wykonana w całości\n");

    return 0;
}