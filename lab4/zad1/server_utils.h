#ifndef SERVER_UTILS_H
#define SERVER_UTILS_H

#include "common.h"

typedef struct {
    int sock;
    char name[MAX_NAME];
    int active;
    int alive;
} Client;

extern Client clients[];

void send_to_client(int sock, char *msg);
void send_to_all(char *msg, int sender_sock);
int find_client(char *name);
int add_client(int sock, char *name);
void remove_client(int sock);
int parse_command(char *buffer);

#endif