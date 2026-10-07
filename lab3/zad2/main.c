#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define FIFO_IN "fifo_in"
#define FIFO_OUT "fifo_out"

int main() {

    double a, b, dx;

    // Wczytanie danych od użytkownika 
    printf("Podaj a b dx: ");
    scanf("%lf %lf %lf", &a, &b, &dx);

    // Tworzenie FIFO 
    mkfifo(FIFO_IN, 0666);
    mkfifo(FIFO_OUT, 0666);


    

    // Otwieramy FIFO do zapisu 
    int fd_in = open(FIFO_IN, O_WRONLY);
    if (fd_in == -1) {
        perror("open fifo_in");
        exit(1);
    }

    // Wysyłamy dane 
    write(fd_in, &a, sizeof(double));
    write(fd_in, &b, sizeof(double));
    write(fd_in, &dx, sizeof(double));

    close(fd_in);

    // Otwieramy FIFO do odczytu 
    int fd_out = open(FIFO_OUT, O_RDONLY);
    if (fd_out == -1) {
        perror("open fifo_out");
        exit(1);
    }

    double result;
    read(fd_out, &result, sizeof(double));

    close(fd_out);

    printf("Wynik całki = %.10f\n", result);

    return 0;
}