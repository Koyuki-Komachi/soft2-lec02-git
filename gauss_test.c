#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct point{
    double x;
    double y;
} Point;

int main(){
    for (int i = 0; i < 10000; ++i) {
        Point p;
        double a = (double)rand() / RAND_MAX;
        double b = (double)rand() / RAND_MAX;
        p.x = sqrt(-2 * log(a)) * cos(2 * M_PI * b);
        p.y = sqrt(-2 * log(a)) * sin(2 * M_PI * b);
        printf("%f %f\n", p.x, p.y);
    }
}