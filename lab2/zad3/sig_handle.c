#include <stdio.h>
#include <signal.h>
#include "signals.h"


void handler(int sig) {
    printf("Wywołano handler dla sygnału %d\n", sig);
}

void sig_handle() {
    printf("Wywołano funkcję 'sig_handle()'\n");
    signal(SIGUSR1, handler);
}
