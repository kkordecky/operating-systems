#include "integral.h"

// Funckja w całce
double f(double x) {
    return 4.0 / (x * x + 1.0);
}

// Liczenie całki metodą prostokątów 
double compute(double a, double b, double dx) {
    double sum = 0.0;

    for (double x = a; x < b; x += dx) {
        sum += f(x) * dx;
    }

    return sum;
}