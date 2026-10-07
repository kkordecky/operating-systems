#ifndef COMMON_H
#define COMMON_H

#include <semaphore.h>

#define BUFFER_SIZE 10
#define TASK_SIZE 11

#define PRODUCERS 3
#define CONSUMERS 2

#define TASKS_PER_PRODUCER 10

typedef struct {

    char buffer[BUFFER_SIZE][TASK_SIZE];

    int in;
    int out;

    // semafory bufora
    sem_t empty; // czy jest miejsce wolne w buforze
    sem_t full; // ile gotowych do odczytu
    sem_t mutex;

    // semafor do printf
    sem_t print_mutex;

} shared_buffer;

#endif