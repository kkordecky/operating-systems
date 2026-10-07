#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

int parse_mode(char *arg) {
    if (strcmp(arg, "default") == 0) return 0;
    if (strcmp(arg, "ignore") == 0) return 1;
    if (strcmp(arg, "handle") == 0) return 2;
    if (strcmp(arg, "mask") == 0) return 3;
    return -1;
}

int main(int argc, char *argv[]) {

    // ==== WERYFIKACJA AGRMENTÓW =====
    if (argc != 2) {
        printf("Wywołanie: %s default|mask|ignore|handle\n", argv[0]);
        return 1;
    }

    int mode = parse_mode(argv[1]);

    if (mode == -1) {
        printf("Niepoprawny argument!\n");
        return 1;
    }

    // ==== TWORZENIE POTOMKA ====
    pid_t pid = fork();

    if (pid == 0) {
        execl("./child", "./child", NULL);
        perror("execl");
        exit(1);
    }

    sleep(1); // Oczekiwanie na inicjacje procesu potomka

    // ==== NADANIE SYGNAŁU SIGUSR2 do potomka ==== 

    union sigval val;
    val.sival_int = mode;

    printf("Parent: wysyłam SIGUSR2\n");
    sigqueue(pid, SIGUSR2, val);

    wait(NULL);

    return 0;
}