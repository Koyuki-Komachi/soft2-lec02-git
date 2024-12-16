#include "darts.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

#define RADIUS 20


// 初期化して円を表示する
void my_init_board(Board *b) {
    for (int y = 0; y <= 2 * RADIUS; ++y) {
        for (int x = 0; x < 4 * RADIUS; ++x) {
            double distance = sqrt((x / 2.0 - RADIUS) * (x / 2.0 - RADIUS) + (y - RADIUS) * (y - RADIUS));
            if (distance < RADIUS + 0.5 && distance > RADIUS - 0.5) {
                b->space[y][x] = '+';
            }else {
                b->space[y][x] = ' ';
            }
            b->space[y][4 * RADIUS] = '\0';
            b->space[y][4 * RADIUS + 1] = '\n';
        }
    }
}

// 盤面を表示
void my_print_board(Board *b) {
    for (int y = 0; y <= 2 * RADIUS; ++y) {
        for (int x = 0; x < 4 * RADIUS + 2; ++x) {
            printf("%c", b->space[y][x]);
        }
    }
}

// 分散が等方向一定の正規分布の乱数を生成する
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

// i回目 (1-3) が盤面内なら数字でプロット
void my_plot_throw(Board *b, Point p, int i) {
    if (p.x + 20 > 0 && p.x + 20 < 40 && p.y + 20 > 0 && p.y + 20 < 40) {
        char char_i = i + '0';
        b->space[(int)(p.y + 20)][(int)(2 * (p.x + 20))] = char_i;
    }
}

// 座標が有効（得点圏内）ならtrueを返す
bool my_is_valid_point(Board *b, Point p) {
    double distance = sqrt(p.x * p.x + p.y * p.y);
    if (distance < 20) {
        return true;
    }else {
        return false;
    }
}

// 座標を (? ?) で表示（改行なし）
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
