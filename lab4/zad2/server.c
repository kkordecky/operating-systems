#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#include <sys/ipc.h>
#include <sys/msg.h>

#include "common.h"
#include "server_utils.h"

// id kolejki serwera
int server_queue;


// ===== handler ctrl+c =====
void sigint_handler(int sig) {

    // usunięcie kolejki serwera
    msgctl(server_queue, IPC_RMID, NULL);

    // usunięcie klientów
    remove_clients();

    printf("\nSerwer zamknięty\n");

    exit(0);
}


int main() {

    // obsługa ctrl+c
    signal(SIGINT, sigint_handler);

    // utworzenie klucza serwera
    key_t key = ftok(
        SERVER_PATH,
        PROJECT_ID
    );

    // utworzenie kolejki serwera
    server_queue = msgget(
        key,
        IPC_CREAT | 0666
    );

    printf("Serwer uruchomiony\n");

    message msg;

    // główna pętla serwera
    while (1) {

        // odbiór wiadomości
        msgrcv(
            server_queue,
            &msg,
            sizeof(message) - sizeof(long),
            0,
            0
        );

        // ===== OBSŁUGA INIT =====
        if (msg.mtype == INIT) {

            // dodanie klienta
            int id = add_client(msg.queue_key);

            printf("Nowy klient: %d\n", id);

            // wiadomość zwrotna
            message reply;

            reply.mtype = INIT;
            reply.sender_id = id;

            // otwarcie kolejki klienta
            int client_queue =
                msgget(msg.queue_key, 0);

            // odesłanie ID klienta
            msgsnd(
                client_queue,
                &reply,
                sizeof(message) - sizeof(long),
                0
            );
        }

        // ===== normalna wiadomość =====
        else if (msg.mtype == MSG) {

            printf(
                "Od %d: %s",
                msg.sender_id,
                msg.text
            );

            // rozesłanie wiadomości
            send_to_all(&msg);
        }
    }

    return 0;
}