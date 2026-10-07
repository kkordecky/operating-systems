#ifndef SERVER_UTILS_H
#define SERVER_UTILS_H

#include "common.h"

// struktura klienta
typedef struct client {

    // czy aktywny
    int active;

    // id klienta
    int id;

    // id kolejki klienta
    int queue_id;

} client;

// tablica klientów
extern client clients[MAX_CLIENTS];

// dodanie klienta
int add_client(key_t key);

// wysłanie do wszystkich
void send_to_all(message *msg);

// czyszczenie klientów
void remove_clients();

#endif