#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
//#include "darts.h"
#include <unistd.h>
#include <time.h>

#define RADIUS 20

typedef struct point{
    double x;
    double y;
} Point;

typedef struct board{
    char space[2*RADIUS + 1][4 * RADIUS + 3];
    double radius;
} Board;

void my_init_board(Board *b) {
    for (int y = -RADIUS; y <= RADIUS; ++y) {
        for (int x = -2 * RADIUS - 1; x <= 2 * RADIUS + 1; ++x) {
            b->radius = sqrt((x / 2.0) * (x / 2.0)  + y * y);
            if (b->radius < RADIUS + 0.5 && b->radius > RADIUS - 0.5) {
                b->space[y][x] = '+';
                printf("%c", b->space[y][x]);
            }else {
                b->space[y][x] = ' ';
                printf("%c", b->space[y][x]);
            }
        }
        printf("\n");
    }
}

void my_print_board(Board *b) {
    for (int y = -RADIUS; y <= RADIUS; ++y) {
        for (int x = -2 * RADIUS -1; x <= 2 * RADIUS + 1; ++x) {
            printf("%c", b->space[y][x]);
        }
        printf("\n");
    }
}

Point my_iso_gauss_rand(Point mu, double stddev) {
    Point p;
    double u1 = (double)rand() / RAND_MAX;
    double u2 = (double)rand() / RAND_MAX;
    double X = sqrt(-2 * log(u1)) * cos(2 * M_PI * u2);
    double Y = sqrt(-2 * log(u1)) * sin(2 * M_PI * u2);
    p.x = stddev * X + mu.x;
    p.y = stddev * Y + mu.y;
    return p;
}

void my_plot_throw(Board *b, Point p, int i) {
    if (p.x > -20 && p.x < 20 && p.y > -20 && p.y < 20) {
        b->space[(int)p.y][(int)p.x * 2] = 'i';
    }
}

bool my_is_valid_point(Board *b, Point p) {
    double distance = sqrt(p.x * p.x + p.y * p.y);
    if (distance < 20) {
        return true;
    }else {
        return false;
    }
}

void my_print_point(Point p) {
    printf("(%f %f)", p.x, p.y);
}

int main(int argc, char **argv){
    Board board;

    srand((unsigned int)time(NULL));

    my_init_board(&board);

    // 3回投げる
    for (int i = 1 ; i <= 3 ; i++){
        Point p = my_iso_gauss_rand((Point){.x = 0, .y = 0}, 15.0);

        my_plot_throw(&board,p,i);
        my_print_board(&board);
        printf("-------\n");
        my_print_point(p);
        if (!my_is_valid_point(&board, p)) printf(" miss!");
        printf("\n");
        sleep(1);
    }
    return 0;
}
