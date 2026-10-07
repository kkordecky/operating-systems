#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "common.h"

// wypisywanie znak po znaku
void print_task(shared_buffer *buf,
                char *task,
                int id) {

    sem_wait(&buf->print_mutex);

    printf(
        "KONSUMENT %d pobrał: ",
        id
    );

    // Wypisywanie znak po znaku 
    for (int i = 0; i < 10; i++) {

        printf("%c", task[i]);

        fflush(stdout);

        // odczekaj 0.3 s 
        usleep(300000);
    }

    printf("\n");

    sem_post(&buf->print_mutex);
}

// konsument
void consumer(shared_buffer *buf, int id) {

    while (1) {

        char task[TASK_SIZE];

        // czekaj na dane, aż nie pojawią się dane w buforze
        sem_wait(&buf->full);

        // sekcja krytyczna
        sem_wait(&buf->mutex);

        // pobranie zadania
        strcpy(
            task,
            buf->buffer[buf->out]
        );

        buf->out =
            (buf->out + 1)
            % BUFFER_SIZE;

        // wyjście z sekcji krytycznej
        sem_post(&buf->mutex);

        // zwolnienie miejsca
        sem_post(&buf->empty);

        print_task(buf, task, id);

}
}