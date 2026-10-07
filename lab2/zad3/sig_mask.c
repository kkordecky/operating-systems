#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include "signals.h"

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