#ifndef SERVER_HANDLERS_H
#define SERVER_HANDLERS_H

void handle_list(int sock);
void handle_2all(int sender_idx, int sock, char *buffer);
void handle_2one(int sender_idx, char *buffer);
void handle_stop(int sock);
void handle_alive(int sock);

#endif