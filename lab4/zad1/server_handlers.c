#include <stdio.h>
#include <string.h>
#include <time.h>

#include "server_utils.h"


// ===== LIST =====
void handle_list(int sock) {

    char list[MAX_MSG] = "Aktywni klienci:\n";

    // Przeszukujemy całą tablicę klientów
    for (int i = 0; i < MAX_CLIENTS; i++) {

        // Jeśli klient jest aktywny (active == 1)
        if (clients[i].active) {

            // Dodajemy jego nazwę do listy
            strcat(list, clients[i].name);
            strcat(list, "\n");
        }
    }

    // Wysyłamy listę do klienta
    send_to_client(sock, list);
}


// ===== 2ALL =====
void handle_2all(int sender_idx,
                 int sock,
                 char *buffer) {

    char msg[MAX_MSG];

    time_t now = time(NULL);

    // pobranie daty
    char *date = ctime(&now);

    // usunięcie '\n' z końca
    date[strlen(date) - 1] = '\0';

    // Formatujemy wiadomość końcową
    snprintf(
        msg,
        MAX_MSG,
        "[%s] \n %s: %s\n",
        date,
        clients[sender_idx].name,
        buffer + 5
    );

    // Wywyłamy do wszystkich
    send_to_all(msg, sock);
}


// ===== 2ONE =====
void handle_2one(int sender_idx,
                 char *buffer) {

    char temp[MAX_MSG];

    strcpy(temp, buffer);

    strtok(temp, " ");

    char *target = strtok(NULL, " ");
    char *message = strtok(NULL, "\n");

    if (!target || !message)
        return;

    // Sprawdzam czy target istnieje
    int target_idx = find_client(target);

    if (target_idx == -1)
        return;

    char msg[MAX_MSG];

    time_t now = time(NULL);

    // pobranie daty
    char *date = ctime(&now);
    date[strlen(date) - 1] = '\0';

    snprintf(
        msg,
        MAX_MSG,
        "[%s] %s: %s",
        date,
        clients[sender_idx].name,
        message
    );

    send_to_client(
        clients[target_idx].sock,
        msg
    );
}


// ===== STOP =====
void handle_stop(int sock) {

    remove_client(sock);
}


// ===== ALIVE =====
// Funkcja obsługująca komendę ALIVE – odpowiedź na ping serwera
void handle_alive(int sock) {

    for (int i = 0; i < MAX_CLIENTS; i++) {

        if (clients[i].active &&
            clients[i].sock == sock) {

            clients[i].alive = 1;
        }
    }
}