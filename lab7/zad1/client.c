#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <arpa/inet.h>

#include "common.h"
#include "client_utils.h"

int sock;


// ctrl+c
void sigint_handler(int sig) {
    stop_client();
}


int main(int argc, char *argv[]) {

    // Weryfikacja liczby argumentów + instrukcja obsługi
    if (argc != 4) {

        printf("Usage: %s NAME IP PORT\n",
               argv[0]);

        return 1;
    }

    // Obsługa CTRL + C 
    signal(SIGINT,
           sigint_handler);

    // === Parsowanie argumentów ===
    char *name = argv[1];
    char *ip = argv[2];
    int port = atoi(argv[3]);
    // ================


    // ===== Tworzę socket IPv4 w trybie SOCK_STREAM === 
    sock = socket(AF_INET,
                  SOCK_STREAM,
                  0);

    
    // ===== Tworzę strukturę opisującą adres === 
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET; /* address family: AF_INET */ 
    server_addr.sin_port = htons(port); // numer_portu 

    inet_pton(AF_INET,
              ip,
              &server_addr.sin_addr);
    // ===========================


    // Łączę klienta z serwerem 
    connect(sock,
            (struct sockaddr *)&server_addr,
            sizeof(server_addr));


    // ===== wysłanie loginu =====
    send(sock,
         name,
         strlen(name),
         0);


    // ===== sprawdzenie odpowiedzi =====
    char response[MAX_MSG];

    int bytes = recv(sock,
                     response,
                     MAX_MSG - 1,
                     MSG_DONTWAIT);

    if (bytes > 0) {

        response[bytes] = '\0';

        // jeśli serwer zwrócił ERROR
        if (strncmp(response,
                    "ERROR",
                    5) == 0) {

            printf("%s\n", response);

            close(sock);

            return 1;
        }
    }
    


    // ===== wątek odbierający =====
    pthread_t tid;

    pthread_create(&tid,
                   NULL,
                   receive_thread,
                   NULL);


    char buffer[MAX_MSG];

    while (1) {

        memset(buffer,
               0,
               MAX_MSG);

        if (fgets(buffer,
                  MAX_MSG,
                  stdin) == NULL) {

            break;
        }

        buffer[strcspn(buffer, "\n")] = 0;

        send(sock,
             buffer,
             strlen(buffer),
             0);
    }

    stop_client();

    return 0;
}