/*************************************************************************
> FileName: square-state.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Tue 31 Oct 2023 05:23:30 PM CST
 ************************************************************************/
#include "square-state.h"

#include <stdio.h>
#include <stdbool.h>

#define SQUARE_FILE_PATH        "/tmp/square.state"

const char* saveSquareState (Square* square)
{
    FILE* sFile = fopen (SQUARE_FILE_PATH, "wb");
    if (!sFile) {
        printf("file '%s' open error!", SQUARE_FILE_PATH);
        return NULL;
    }

    // store state
    if (fwrite(&square->color, sizeof(SquareColor), 1, sFile) != 1) {
        printf ("store state error!");
        return NULL;
    }

    // close file
    if (fclose (sFile) != 0) {
        printf ("fclose error!");
    }

    return SQUARE_FILE_PATH;
}


void restoreSquareState(const char* pathStr, Square* square)
{
    FILE* sFile = fopen (pathStr, "wb");
    if (!sFile) {
        printf("file '%s' open error!", SQUARE_FILE_PATH);
        return;
    }

    // store state
    if (fread(&square->color, sizeof(SquareColor), 1, sFile) != 1) {
        printf ("read state error!");
        return;
    }

    // close file
    if (fclose (sFile) != 0) {
        printf ("fclose error!");
    }
}
