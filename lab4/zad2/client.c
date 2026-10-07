#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#include <sys/ipc.h>
#include <sys/msg.h>

#include "common.h"
#include "client_utils.h"

// id kolejki klienta
int client_queue;


// ===== ctrl+c =====
void sigint_handler(int sig) {

    // usunięcie kolejki klienta
    msgctl(client_queue, IPC_RMID, NULL);

    printf("\nKlient zamknięty\n");

    exit(0);
}


int main() {

    // obsługa ctrl+c
    signal(SIGINT, sigint_handler);

    // ===== tworzenie kolejki klienta =====

    // unikalny klucz
    key_t client_key = ftok(
        "./client.c",
        getpid()
    );

    // utworzenie kolejki
    client_queue = msgget(
        client_key,
        IPC_CREAT | 0666
    );

    // ===== połączenie z serwerem =====

    // klucz serwera
    key_t server_key = ftok(
        SERVER_PATH,
        PROJECT_ID
    );

    // otwarcie kolejki serwera
    int server_queue = msgget(
        server_key,
        0
    );

    // ===== INIT =====

    message init_msg;

    // typ INIT
    init_msg.mtype = INIT;

    // wysłanie klucza kolejki klienta
    init_msg.queue_key = client_key;

    // wysłanie INIT do serwera
    msgsnd(
        server_queue,
        &init_msg,
        sizeof(message) - sizeof(long),
        0
    );

    // ===== odbiór ID =====

    message reply;

    // odbiór odpowiedzi serwera
    msgrcv(
        client_queue,
        &reply,
        sizeof(message) - sizeof(long),
        INIT,
        0
    );

    // zapis ID klienta
    int client_id = reply.sender_id;

    printf("Mój ID: %d\n", client_id);

    // ===== fork =====

    pid_t pid = fork();

    // ===== proces dziecka =====
    // odbiera wiadomości
    if (pid == 0) {

        receive_messages(client_queue);
    }

    // ===== proces rodzica =====
    // wysyła wiadomości
    else {

        char buffer[MAX_TEXT];

        while (1) {

            // pobranie wiadomości
            if (fgets(buffer, MAX_TEXT, stdin) == NULL) {
                break;
            }

            message msg;

            // typ wiadomości
            msg.mtype = MSG;

            // id nadawcy
            msg.sender_id = client_id;

            // treść wiadomości
            strcpy(msg.text, buffer);

            // wysłanie wiadomości
            msgsnd(
                server_queue,
                &msg,
                sizeof(message) - sizeof(long),
                0
            );
        }
    }

    return 0;
}