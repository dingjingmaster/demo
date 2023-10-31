/*************************************************************************
> FileName: square-state.h
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Tue 31 Oct 2023 05:20:33 PM CST
 ************************************************************************/
#ifndef _SQUARE_STATE_H
#define _SQUARE_STATE_H
#include <stdbool.h>

typedef enum {RedSquare = 0, BlueSquare = 1} SquareColor;

typedef struct
{
    int         x;
    int         y;
    int         mouseX;
    int         mouseY;
    int         size;
    bool        visible;
    bool        selected;
    SquareColor color;
} Square;

const char* saveSquareState (Square* square);
void restoreSquareState(const char* pathStr, Square* square);

#endif
