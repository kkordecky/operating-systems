#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "integral.h"

#define FIFO_IN "fifo_in"
#define FIFO_OUT "fifo_out"

int main() {

    double a, b, dx;

    // Otwieramy FIFO w trybie odczytu
    int fd_in = open(FIFO_IN, O_RDONLY);
    if (fd_in == -1) {
        perror("open fifo_in");
        exit(1);
    }

    // Odczytujemy dane 
    read(fd_in, &a, sizeof(double));
    read(fd_in, &b, sizeof(double));
    read(fd_in, &dx, sizeof(double));

    close(fd_in);

    // Liczymy całkę 
    double result = compute(a, b, dx);

    // Otwieramy FIFO do zapisu 
    int fd_out = open(FIFO_OUT, O_WRONLY);
    if (fd_out == -1) {
        perror("open fifo_out");
        exit(1);
    }

    // Wysyłamy wynik  
    write(fd_out, &result, sizeof(double));
    close(fd_out);

    return 0;
}