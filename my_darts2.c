#include "darts.h"
#include <bits/getopt_core.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

#define RADIUS 20

/*
darts.hの中で、PointとBoardは次のように定義されています。
typedef struct point{
    double x;
    double y;
} Point;

typedef struct board{
    char space[2*RADIUS + 1][4 * RADIUS + 3];
    double radius;
} Board;
*/

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

//角度によってシングルの点数を計算する
char score_radian(Point p) {
    char score_radian = 0;
    double radian = atan2(p.y, p.x);
    if (radian > -M_PI && radian <= -M_PI + M_PI / 20) {
        score_radian = 11;
    }else if (radian > -M_PI + M_PI / 20 && radian <= -M_PI + M_PI * 3 / 20) {
        score_radian = 14;
    }else if (radian > -M_PI + M_PI * 3 / 20 && radian <= -M_PI + M_PI * 5 / 20) {
        score_radian = 9;
    }else if (radian > -M_PI + M_PI * 5 / 20 && radian <= -M_PI + M_PI * 7 / 20) {
        score_radian = 12;
    }else if (radian > -M_PI + M_PI * 7 / 20 && radian <= -M_PI + M_PI * 9 / 20) {
        score_radian = 5;
    }else if (radian > -M_PI + M_PI * 9 / 20 && radian <= -M_PI + M_PI * 11 / 20) {
        score_radian = 20;
    }else if (radian > -M_PI + M_PI * 11 / 20 && radian <= -M_PI + M_PI * 13 / 20) {
        score_radian = 1;
    }else if (radian > -M_PI + M_PI * 13 / 20 && radian <= -M_PI + M_PI * 15 / 20) {
        score_radian = 18;
    }else if (radian > -M_PI + M_PI * 15 / 20 && radian <= -M_PI + M_PI * 17 / 20) {
        score_radian = 4;
    }else if (radian > -M_PI + M_PI * 17 / 20 && radian <= -M_PI + M_PI * 19 / 20) {
        score_radian = 13;
    }else if (radian > -M_PI + M_PI * 19 / 20 && radian <= -M_PI + M_PI * 21 / 20) {
        score_radian = 6;
    }else if (radian > -M_PI + M_PI * 21 / 20 && radian <= -M_PI + M_PI * 23 / 20) {
        score_radian = 10;
    }else if (radian > -M_PI + M_PI * 23 / 20 && radian <= -M_PI + M_PI * 25 / 20) {
        score_radian = 15;
    }else if (radian > -M_PI + M_PI * 25 / 20 && radian <= -M_PI + M_PI * 27 / 20) {
        score_radian = 2;
    }else if (radian > -M_PI + M_PI * 27 / 20 && radian <= -M_PI + M_PI * 29 / 20) {
        score_radian = 17;
    }else if (radian > -M_PI + M_PI * 29 / 20 && radian <= -M_PI + M_PI * 31 / 20) {
        score_radian = 3;
    }else if (radian > -M_PI + M_PI * 31 / 20 && radian <= -M_PI + M_PI * 33 / 20) {
        score_radian = 19;
    }else if (radian > -M_PI + M_PI * 33 / 20 && radian <= -M_PI + M_PI * 35 / 20) {
        score_radian = 7;
    }else if (radian > -M_PI + M_PI * 35 / 20 && radian <= -M_PI + M_PI * 37 / 20) {
        score_radian = 16;
    }else if (radian > -M_PI + M_PI * 37 / 20 && radian <= -M_PI + M_PI * 39 / 20) {
        score_radian = 8;
    }else if (radian > -M_PI + M_PI * 39 / 20 && radian <= -M_PI + M_PI * 40 / 20) {
        score_radian = 11;
    }else {
        printf("somthing is wrong with radian");
        exit(EXIT_FAILURE);
    }
    return score_radian;
}

// 得点を計算する
char score(Point p) {
    char score = 0;
    double r = sqrt(p.x * p.x + p.y * p.y);
    if (r <= 1) { // インナーブル
        score = 50;
    }else if (r > 1 && r <= 3) { // アウターブル
        score = 25;
    }else if (r > 3 && r <= 11) { // 内シングル
        score = score_radian(p);
    }else if (r > 11 && r <= 13) { // トリプル
        score = 3 * score_radian(p);
    }else if (r > 13 && r <= 18) { // 外シングル
        score = score_radian(p);
    }else if (r > 18 && r <= 20) { // ダブル
        score = 2 + score_radian(p);
    }else {
        score = 0;
    }
    return score;
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

// ユーザの入力によって、正規分布の中心を指定する
Point gauss_various_center(char *s) {
    Point center = {.x = 0, .y = 0};
    if (s == NULL) {
        printf("center designation is missing\n");
        exit(EXIT_FAILURE);
    }
    // ブルを狙った場合
    else if (strcmp(s, "B") == 0) {
        center.x = 0;
        center.y = 0;
    }
    // トリプルを狙っ場合
    else if (strcmp(s, "T6") == 0) {
        center.x = 12;
        center.y = 0;
    }else if (strcmp(s, "T10") == 0) {
        center.x = 12 * cos(M_PI / 10);
        center.y = 12 * sin(M_PI / 10);
    }else if (strcmp(s, "T15") == 0) {
        center.x = 12 * cos(M_PI * 2 / 10);
        center.y = 12 * sin(M_PI * 2 / 10);
    }else if (strcmp(s, "T2") == 0) {
        center.x = 12 * cos(M_PI * 3 / 10);
        center.y = 12 * sin(M_PI * 3 / 10);
    }else if (strcmp(s, "T17") == 0) {
        center.x = 12 * cos(M_PI * 4 / 10);
        center.y = 12 * sin(M_PI * 4 / 10);
    }else if (strcmp(s, "T3") == 0) {
        center.x = 12 * cos(M_PI * 5 / 10);
        center.y = 12 * sin(M_PI * 5 / 10);
    }else if (strcmp(s, "T19") == 0) {
        center.x = 12 * cos(M_PI * 6 / 10);
        center.y = 12 * sin(M_PI * 6 / 10);
    }else if (strcmp(s, "T7") == 0) {
        center.x = 12 * cos(M_PI * 7 / 10);
        center.y = 12 * sin(M_PI * 7 / 10);
    }else if (strcmp(s, "T16") == 0) {
        center.x = 12 * cos(M_PI * 8 / 10);
        center.y = 12 * sin(M_PI * 8 / 10);
    }else if (strcmp(s, "T8") == 0) {
        center.x = 12 * cos(M_PI * 9 / 10);
        center.y = 12 * sin(M_PI * 9 / 10);
    }else if (strcmp(s, "T11") == 0) {
        center.x = 12 * cos(M_PI);
        center.y = 12 * sin(M_PI);
    }else if (strcmp(s, "T14") == 0) {
        center.x = 12 * cos(M_PI * 11 / 10);
        center.y = 12 * sin(M_PI * 11 / 10);
    }else if (strcmp(s, "T9") == 0) {
        center.x = 12 * cos(M_PI * 12 / 10);
        center.y = 12 * sin(M_PI * 12 / 10);
    }else if (strcmp(s, "T12") == 0) {
        center.x = 12 * cos(M_PI * 13 / 10);
        center.y = 12 * sin(M_PI * 13 / 10);
    }else if (strcmp(s, "T5") == 0) {
        center.x = 12 * cos(M_PI * 14 / 10);
        center.y = 12 * sin(M_PI * 14 / 10);
    }else if (strcmp(s, "T20") == 0) {
        center.x = 12 * cos(M_PI * 15 / 10);
        center.y = 12 * sin(M_PI * 15 / 10);
    }else if (strcmp(s, "T1") == 0) {
        center.x = 12 * cos(M_PI * 16 / 10);
        center.y = 12 * sin(M_PI * 16 / 10);
    }else if (strcmp(s, "T18") == 0) {
        center.x = 12 * cos(M_PI * 17 / 10);
        center.y = 12 * sin(M_PI * 17 / 10);
    }else if (strcmp(s, "T4") == 0) {
        center.x = 12 * cos(M_PI * 18 / 10);
        center.y = 12 * sin(M_PI * 18 / 10);
    }else if (strcmp(s, "T13") == 0) {
        center.x = 12 * cos(M_PI * 19 / 10);
        center.y = 12 * sin(M_PI * 19 / 10);
    }
    // ダブルを狙った場合
    else if (strcmp(s, "D6") == 0) {
        center.x = 19;
        center.y = 0;
    }else if (strcmp(s, "D10") == 0) {
        center.x = 19 * cos(M_PI / 10);
        center.y = 19 * sin(M_PI / 10);
    }else if (strcmp(s, "D15") == 0) {
        center.x = 19 * cos(M_PI * 2 / 10);
        center.y = 19 * sin(M_PI * 2 / 10);
    }else if (strcmp(s, "D2") == 0) {
        center.x = 19 * cos(M_PI * 3 / 10);
        center.y = 19 * sin(M_PI * 3 / 10);
    }else if (strcmp(s, "D17") == 0) {
        center.x = 19 * cos(M_PI * 4 / 10);
        center.y = 19 * sin(M_PI * 4 / 10);
    }else if (strcmp(s, "D3") == 0) {
        center.x = 19 * cos(M_PI * 5 / 10);
        center.y = 19 * sin(M_PI * 5 / 10);
    }else if (strcmp(s, "D19") == 0) {
        center.x = 19 * cos(M_PI * 6 / 10);
        center.y = 19 * sin(M_PI * 6 / 10);
    }else if (strcmp(s, "D7") == 0) {
        center.x = 19 * cos(M_PI * 7 / 10);
        center.y = 19 * sin(M_PI * 7 / 10);
    }else if (strcmp(s, "D16") == 0) {
        center.x = 19 * cos(M_PI * 8 / 10);
        center.y = 19 * sin(M_PI * 8 / 10);
    }else if (strcmp(s, "D8") == 0) {
        center.x = 19 * cos(M_PI * 9 / 10);
        center.y = 19 * sin(M_PI * 9 / 10);
    }else if (strcmp(s, "D11") == 0) {
        center.x = 19 * cos(M_PI);
        center.y = 19 * sin(M_PI);
    }else if (strcmp(s, "D14") == 0) {
        center.x = 19 * cos(M_PI * 11 / 10);
        center.y = 19 * sin(M_PI * 11 / 10);
    }else if (strcmp(s, "D9") == 0) {
        center.x = 19 * cos(M_PI * 12 / 10);
        center.y = 19 * sin(M_PI * 12 / 10);
    }else if (strcmp(s, "D12") == 0) {
        center.x = 19 * cos(M_PI * 13 / 10);
        center.y = 19 * sin(M_PI * 13 / 10);
    }else if (strcmp(s, "D5") == 0) {
        center.x = 19 * cos(M_PI * 14 / 10);
        center.y = 19 * sin(M_PI * 14 / 10);
    }else if (strcmp(s, "D20") == 0) {
        center.x = 19 * cos(M_PI * 15 / 10);
        center.y = 19 * sin(M_PI * 15 / 10);
    }else if (strcmp(s, "D1") == 0) {
        center.x = 19 * cos(M_PI * 16 / 10);
        center.y = 19 * sin(M_PI * 16 / 10);
    }else if (strcmp(s, "D18") == 0) {
        center.x = 19 * cos(M_PI * 17 / 10);
        center.y = 19 * sin(M_PI * 17 / 10);
    }else if (strcmp(s, "D4") == 0) {
        center.x = 19 * cos(M_PI * 18 / 10);
        center.y = 19 * sin(M_PI * 18 / 10);
    }else if (strcmp(s, "D13") == 0) {
        center.x = 19 * cos(M_PI * 19 / 10);
        center.y = 19 * sin(M_PI * 19 / 10);
    }
    // シングルを狙った場合
    else if (strcmp(s, "S6") == 0) {
        center.x = 10;
        center.y = 0;
    }else if (strcmp(s, "D10") == 0) {
        center.x = 10 * cos(M_PI / 10);
        center.y = 10 * sin(M_PI / 10);
    }else if (strcmp(s, "D15") == 0) {
        center.x = 10 * cos(M_PI * 2 / 10);
        center.y = 10 * sin(M_PI * 2 / 10);
    }else if (strcmp(s, "D2") == 0) {
        center.x = 10 * cos(M_PI * 3 / 10);
        center.y = 10 * sin(M_PI * 3 / 10);
    }else if (strcmp(s, "D17") == 0) {
        center.x = 10 * cos(M_PI * 4 / 10);
        center.y = 10 * sin(M_PI * 4 / 10);
    }else if (strcmp(s, "D3") == 0) {
        center.x = 10 * cos(M_PI * 5 / 10);
        center.y = 10 * sin(M_PI * 5 / 10);
    }else if (strcmp(s, "D19") == 0) {
        center.x = 10 * cos(M_PI * 6 / 10);
        center.y = 10 * sin(M_PI * 6 / 10);
    }else if (strcmp(s, "D7") == 0) {
        center.x = 10 * cos(M_PI * 7 / 10);
        center.y = 10 * sin(M_PI * 7 / 10);
    }else if (strcmp(s, "D16") == 0) {
        center.x = 10 * cos(M_PI * 8 / 10);
        center.y = 10 * sin(M_PI * 8 / 10);
    }else if (strcmp(s, "D8") == 0) {
        center.x = 10 * cos(M_PI * 9 / 10);
        center.y = 10 * sin(M_PI * 9 / 10);
    }else if (strcmp(s, "D11") == 0) {
        center.x = 10 * cos(M_PI);
        center.y = 10 * sin(M_PI);
    }else if (strcmp(s, "D14") == 0) {
        center.x = 10 * cos(M_PI * 11 / 10);
        center.y = 10 * sin(M_PI * 11 / 10);
    }else if (strcmp(s, "D9") == 0) {
        center.x = 10 * cos(M_PI * 12 / 10);
        center.y = 10 * sin(M_PI * 12 / 10);
    }else if (strcmp(s, "D12") == 0) {
        center.x = 10 * cos(M_PI * 13 / 10);
        center.y = 10 * sin(M_PI * 13 / 10);
    }else if (strcmp(s, "D5") == 0) {
        center.x = 10 * cos(M_PI * 14 / 10);
        center.y = 10 * sin(M_PI * 14 / 10);
    }else if (strcmp(s, "D20") == 0) {
        center.x = 10 * cos(M_PI * 15 / 10);
        center.y = 10 * sin(M_PI * 15 / 10);
    }else if (strcmp(s, "D1") == 0) {
        center.x = 10 * cos(M_PI * 16 / 10);
        center.y = 10 * sin(M_PI * 16 / 10);
    }else if (strcmp(s, "D18") == 0) {
        center.x = 10 * cos(M_PI * 17 / 10);
        center.y = 10 * sin(M_PI * 17 / 10);
    }else if (strcmp(s, "D4") == 0) {
        center.x = 10 * cos(M_PI * 18 / 10);
        center.y = 10 * sin(M_PI * 18 / 10);
    }else if (strcmp(s, "D13") == 0) {
        center.x = 10 * cos(M_PI * 19 / 10);
        center.y = 10 * sin(M_PI * 19 / 10);
    }
    // 上記以外の文字列が打ち込まれた場合
    else {
        printf("center designation is invalid\n");
        exit(EXIT_FAILURE);
    }
    return center;
}

int main(int argc, char **argv){
    Board board;
    srand((unsigned int)time(NULL));
    my_init_board(&board);

    // -vオプションから標準偏差を読み取る
    char *stddev = NULL;
    int opt;
    while ((opt = getopt(argc, argv, "v:")) != -1) {
        switch (opt) {
            case 'v':
                stddev = optarg;
                break;
            case '?':
                if (optopt == 'v') {
                    fprintf(stderr, "Option -%c requires a standard deviation.\n", optopt);
                }else {
                    fprintf(stderr, "Unknown option -%c.\n", optopt);
                }
                return 1;
        }
    }
    if (stddev == NULL) {
        fprintf(stderr, "Standard deviation (-v option) is required.\n");
        return 1;
    }
    if (argc - optind < 3) {
        fprintf(stderr, "Three throwing targets are requires.\n");
        return 1;
    }

    // 3回投げる
    char sum_score = 0;
    for (int i = 0 ; i < 3 ; i++){
        // (i + 1)回目の狙いをarg[optind + i]に書き込む
        Point center = gauss_various_center(argv[optind + i]);
        Point p = my_iso_gauss_rand(center, atoi(stddev));

        my_plot_throw(&board,p,i + 1);
        my_print_board(&board);
        printf("-------\n");
        my_print_point(p);
        if (!my_is_valid_point(&board, p)) printf(" miss!\n");
        printf(" %d回目のスコアは%dです\n", i + 1, score(p));
        sum_score += score(p);
        sleep(1);
    }
    printf("\n");
    printf("合計のスコアは%dです\n", sum_score);
    return 0;
}
