#include <stdio.h>
#include <signal.h>
#include "signals.h"

void sig_default() {
    printf("Wywołano funkcję 'sig_default()'\n");
    signal(SIGUSR1, SIG_DFL);
}
