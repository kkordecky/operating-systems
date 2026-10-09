#ifndef COMMON_H
#define COMMON_H

#include <sys/types.h>

// maksymalna liczba klientów
#define MAX_CLIENTS 10

// maksymalna długość wiadomości
#define MAX_TEXT 256

// dane do ftok()
#define SERVER_PATH "./server.c"
#define PROJECT_ID 1

// typy wiadomości
#define INIT 1
#define MSG 2

// struktura wiadomości
typedef struct message {

    // typ wiadomości
    long mtype;

    // id nadawcy
    int sender_id;

    // klucz kolejki klienta
    key_t queue_key;

    // treść wiadomości
    char text[MAX_TEXT];

} message;

#endif