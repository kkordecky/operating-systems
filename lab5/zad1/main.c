#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/mman.h>
#include <sys/wait.h>

#include "common.h"

// deklaracje
void producer(shared_buffer *buf, int id);

void consumer(shared_buffer *buf, int id);

int main() {

    // pamięć współdzielona
    shared_buffer *buf = mmap(
        NULL,
        sizeof(shared_buffer),
        PROT_READ | PROT_WRITE,
        MAP_SHARED | MAP_ANONYMOUS,
        -1,
        0
    );

    // inicjalizacja indeksów
    buf->in = 0;
    buf->out = 0;

    // inicjalizacja semaforów
    sem_init(&buf->empty, 1, BUFFER_SIZE);
    sem_init(&buf->full, 1, 0);
    sem_init(&buf->mutex, 1, 1);

    sem_init(&buf->print_mutex, 1, 1);

    // Tworzeni są producenci
    for (int i = 0; i < PRODUCERS; i++) {

        pid_t pid = fork();

        if (pid == 0) {

            producer(buf, i);

            exit(0);
        }
    }

    // Tworzeni są konsumenci
    for (int i = 0; i < CONSUMERS; i++) {

        pid_t pid = fork();

        if (pid == 0) {

            consumer(buf, i);

            exit(0);
        }
    }

    // czekanie na producentów
    for (int i = 0; i < PRODUCERS; i++) {

        wait(NULL);
    }

    while (1) {
        sleep(1);
    }

    return 0;
}