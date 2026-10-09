#include <stdio.h>
#include <sys/msg.h>

#include "server_utils.h"

// tablica klientów
client clients[MAX_CLIENTS];


// ===== dodawanie klienta =====
int add_client(key_t key) {

    for (int i = 0; i < MAX_CLIENTS; i++) {

        // szukamy wolnego miejsca
        if (!clients[i].active) {

            // otwarcie kolejki klienta
            int qid = msgget(key, 0);

            // zapis danych klienta
            clients[i].active = 1;
            clients[i].id = i;
            clients[i].queue_id = qid;

            return i;
        }
    }

    return -1;
}


// ===== wysyłanie do wszystkich =====
void send_to_all(message *msg) {

    for (int i = 0; i < MAX_CLIENTS; i++) {

        // wysyłamy do wszystkich poza nadawcą
        if (clients[i].active &&
            clients[i].id != msg->sender_id) {

            msgsnd(
                clients[i].queue_id,
                msg,
                sizeof(message) - sizeof(long),
                0
            );
        }
    }
}


// ===== usuwanie klientów =====
void remove_clients() {

    for (int i = 0; i < MAX_CLIENTS; i++) {

        clients[i].active = 0;
    }
}