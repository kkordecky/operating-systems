#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include "child.h"

static void handler(int sig) {
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
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);

    sigprocmask(SIG_BLOCK, &set, NULL);
}

void sig_unblock() {
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);

    sigprocmask(SIG_UNBLOCK, &set, NULL);
}
// ==================================

// Wysłanie do siebie SIGUSR1
void send_usr1() {
    printf("Wysyłam sygnał USR1\n");
    raise(SIGUSR1);
}