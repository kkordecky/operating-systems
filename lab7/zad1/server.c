#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <arpa/inet.h>

#include "server_utils.h"
#include "server_handlers.h"

// Mutex globalny do synchronizacji dostępu do tablicy clients[]
pthread_mutex_t clients_mutex =
    PTHREAD_MUTEX_INITIALIZER;


// wątek pingujący klientów
void *alive_thread(void *arg) {

    while (1) {

        sleep(10);

        // Blokuję dostęp do clients
        pthread_mutex_lock(&clients_mutex);

        for (int i = 0; i < MAX_CLIENTS; i++) {

            if (clients[i].active) {

                // jeśli klient nie odpowiedział => zamykam socketa 
                if (clients[i].alive == 0) {

                    printf("Klient %s nie odpowiada\n",
                           clients[i].name);

                    close(clients[i].sock);
                    clients[i].active = 0;
                }

                else {

                    // zerowanie flagi alive
                    clients[i].alive = 0;

                    // wysłanie pinga
                    send_to_client(clients[i].sock,
                                   "PING");
                }
            }
        }

        // Odblokowuję mutex
        pthread_mutex_unlock(&clients_mutex);
    }

    return NULL;
}


// obsługa klienta
void *client_thread(void *arg) {

    int sock = *(int *)arg; // Pobieram gniazdo klienta
    free(arg); // Zwalniam pamięć zaalokowaną na wskaźnik

    char buffer[MAX_MSG];

    // pierwsza wiadomość = nazwa klienta
    int bytes = recv(sock, buffer, MAX_MSG - 1, 0);
    buffer[bytes] = '\0';
    

    // Dodaję klienta do globalnej tablicy clients[]
    int idx = add_client(sock, buffer);

    // Jeśli klient o tej nazwie już istnieje => zamykam połączenie
    if (idx == -1) {

        send_to_client(sock,
                       "ERROR: nazwa zajęta\n");

        close(sock);

        return NULL;
    }

    // Zgłoszenie nowego koienta
    printf("Nowy klient: %s\n",
           clients[idx].name);


    // Pętla odbierająca komendy od klienta 
    while (1) {


        memset(buffer, 0, MAX_MSG);

        int bytes = recv(sock,
                         buffer,
                         MAX_MSG,
                         0);

        if (bytes <= 0) {

            remove_client(sock);
            break;
        }
        
        // Parsuję komendę i odpowiednio ją obsługuję 
        switch(parse_command(buffer)) {

            case CMD_LIST:
                handle_list(sock);
                break;

            case CMD_2ALL:
                handle_2all(idx,
                            sock,
                            buffer);
                break;

            case CMD_2ONE:
                handle_2one(idx,
                            buffer);
                break;

            case CMD_STOP:
                handle_stop(sock);
                return NULL;

            case CMD_ALIVE:
                handle_alive(sock);
                break;

            default:
                send_to_client(sock,
                               "Nieznane polecenie\n");
        }
    }

    return NULL;
}


int main(int argc, char *argv[]) {
    // zapisywanie do zamkniętego gniazda nie przerywa programu
    signal(SIGPIPE, SIG_IGN);

    // Weryfikacja argumentu
    if (argc != 2) {

        printf("Usage: %s PORT\n",
               argv[0]);

        return 1;
    }


    // ==== Uruchamiam serwer ==== 
    int port = atoi(argv[1]);

    int server_sock =
    socket(AF_INET,
               SOCK_STREAM,
               0);

    struct sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_sock,
         (struct sockaddr *)&server_addr,
         sizeof(server_addr));

    listen(server_sock,
           MAX_CLIENTS); // Maksymalna kolejka oczekujących połączeń

    printf("Serwer działa na porcie %d\n",
           port);
    // == Serewer uruchomiony i nasłuchuje na porcie ====


    // uruchomienie wątku ALIVE
    pthread_t alive_tid;
    pthread_create(&alive_tid,
                   NULL,
                   alive_thread,
                   NULL);


    // Główna pętla serwera – akceptuje nowych klientów
    while (1) {

        struct sockaddr_in client_addr;

        socklen_t client_len =
            sizeof(client_addr);

        int client_sock = accept(
            server_sock,
            (struct sockaddr *)&client_addr,
            &client_len
        );

        // Alokuję pamięć na gniazdo, żeby przekazać je do wątku
        int *pclient = malloc(sizeof(int));
        *pclient = client_sock;

        pthread_t tid;

        pthread_create(
            &tid,
            NULL,
            client_thread,
            pclient
        );

        pthread_detach(tid); // Odłączam wątek – sam zwolni zasoby po zakończeniu
    }

    return 0;
}