#pragma once
#include <stdlib.h>
#include <stdbool.h>

#define RADIUS 20

// 座標を表す. [-20,20] が描画範囲
typedef struct point{
    double x;
    double y;
} Point;


// 横方向については改行と'\0' も考慮する
typedef struct board{
    char space[2*RADIUS + 1][4 * RADIUS + 3];
    double radius;
} Board;




// ボード構造体内の配列の上限を取得
size_t get_board_height(Board *b);
size_t get_board_width(Board *b);



// 座標が描画領域ならtrueを返す
bool is_in_board(Board *b, Point p);






