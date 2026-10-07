#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

#include "common.h"

extern int sock;


// zamknięcie klienta
void stop_client() {

    send(sock,
         "STOP",
         4,
         0);

    close(sock);

    exit(0);
}


// odbieranie wiadomości
void *receive_thread(void *arg) {

    char buffer[MAX_MSG];

    while (1) {

        memset(buffer,
               0,
               MAX_MSG);

        int bytes = recv(sock,
                         buffer,
                         MAX_MSG,
                         0);

        if (bytes <= 0) {

            printf("Rozłączono z serwerem\n");

            exit(0);
        }

        // odpowiedź na ping serwera
        if (strcmp(buffer, "PING") == 0) {

            send(sock,
                 "ALIVE",
                 5,
                 0);
        }

        else {
            printf("%s\n", buffer);
        }
    }

    return NULL;
}