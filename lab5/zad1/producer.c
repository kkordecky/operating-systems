#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "common.h"

// generowanie losowego stringa
void generate_task(char *task) {

    for (int i = 0; i < 10; i++) {

        task[i] = 'A' + rand() % 26;
    }

    task[10] = '\0';
}


// producent
void producer(shared_buffer *buf, int id) {

    srand(getpid());

    for (int i = 0; i < TASKS_PER_PRODUCER; i++) {

        char task[TASK_SIZE];

        generate_task(task);

        // czekaj na wolne miejsce
        sem_wait(&buf->empty);

        // wejście do sekcji krytycznej
        sem_wait(&buf->mutex);

        // dodanie zadania
        strcpy(
            buf->buffer[buf->in],
            task
        );


        //  Wypisanie informacji o op. producenta
        sem_wait(&buf->print_mutex);

        printf(
            "PRODUCENT %d dodał: %s\n",
            id,
            task
        );

        sem_post(&buf->print_mutex);
        // 

        // Inkrementacja indeksu zczytującego 
        buf->in =
            (buf->in + 1)
            % BUFFER_SIZE;

        // wyjście z sekcji krytycznej
        sem_post(&buf->mutex);

        // zwiększenie liczby pełnych
        sem_post(&buf->full);

        sleep(1);
    }

    exit(0);
}