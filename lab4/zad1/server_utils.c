#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <pthread.h>

#include "server_utils.h"

Client clients[MAX_CLIENTS];

extern pthread_mutex_t clients_mutex;


// wysłanie wiadomości do jednego klienta
void send_to_client(int sock, char *msg) {
    send(sock, msg, strlen(msg), 0);
}


// wysłanie wiadomości do wszystkich klientów
void send_to_all(char *msg, int sender_sock) {

    pthread_mutex_lock(&clients_mutex);

    for (int i = 0; i < MAX_CLIENTS; i++) {

        if (clients[i].active &&
            clients[i].sock != sender_sock) {

            send_to_client(clients[i].sock, msg);
        }
    }

    pthread_mutex_unlock(&clients_mutex);
}


// wyszukiwanie klienta po nazwie
int find_client(char *name) {

    for (int i = 0; i < MAX_CLIENTS; i++) {

        if (clients[i].active &&
            strcmp(clients[i].name, name) == 0) {

            return i;
        }
    }

    return -1;
}


// dodanie klienta
int add_client(int sock, char *name) {

    pthread_mutex_lock(&clients_mutex);

    // sprawdzenie unikalności nazwy
    for (int i = 0; i < MAX_CLIENTS; i++) {

        if (clients[i].active &&
            strcmp(clients[i].name, name) == 0) {

            pthread_mutex_unlock(&clients_mutex);
            return -1;
        }
    }

    // znalezienie wolnego miejsca
    for (int i = 0; i < MAX_CLIENTS; i++) {

        if (!clients[i].active) {

            clients[i].active = 1;
            clients[i].alive = 1;
            clients[i].sock = sock;

            strcpy(clients[i].name, name);

            pthread_mutex_unlock(&clients_mutex);
            return i;
        }
    }

    pthread_mutex_unlock(&clients_mutex);

    return -1;
}


// usuwanie klienta
void remove_client(int sock) {

    pthread_mutex_lock(&clients_mutex);

    for (int i = 0; i < MAX_CLIENTS; i++) {

        if (clients[i].active &&
            clients[i].sock == sock) {

            printf("Usunięto klienta %s\n",
                   clients[i].name);

            clients[i].active = 0;

            close(clients[i].sock);
        }
    }

    pthread_mutex_unlock(&clients_mutex);
}


// rozpoznawanie polecenia
int parse_command(char *buffer) {

    if (strncmp(buffer, "LIST", 4) == 0)
        return CMD_LIST;

    if (strncmp(buffer, "2ALL ", 5) == 0)
        return CMD_2ALL;

    if (strncmp(buffer, "2ONE ", 5) == 0)
        return CMD_2ONE;

    if (strncmp(buffer, "STOP", 4) == 0)
        return CMD_STOP;

    if (strncmp(buffer, "ALIVE", 5) == 0)
        return CMD_ALIVE;

    return CMD_UNKNOWN;
}