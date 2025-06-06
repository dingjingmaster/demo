/*************************************************************************
> FileName: plane-game.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年06月06日 星期五 15时00分47秒
 ************************************************************************/
#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define WIDTH 40
#define HEIGHT 20
#define ENEMY_NUM 5

typedef struct {
    int x, y;
    int alive;
} Enemy;

typedef struct {
    int x, y;
    int active;
} Bullet;

int main() {
    initscr();
    noecho();
    curs_set(FALSE);
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);

    int plane_x = WIDTH / 2, plane_y = HEIGHT - 2;
    Bullet bullet = {0, 0, 0};
    Enemy enemies[ENEMY_NUM];
    int score = 0;
    srand(time(0));

    // 初始化敌机
    for (int i = 0; i < ENEMY_NUM; i++) {
        enemies[i].x = rand() % WIDTH;
        enemies[i].y = rand() % (HEIGHT / 2);
        enemies[i].alive = 1;
    }

    while (1) {
        clear();

        // 画飞机
        mvprintw(plane_y, plane_x, "^");

        // 画子弹
        if (bullet.active) {
            mvprintw(bullet.y, bullet.x, "|");
            bullet.y--;
            if (bullet.y < 0) bullet.active = 0;
        }

        // 画敌机
        for (int i = 0; i < ENEMY_NUM; i++) {
            if (enemies[i].alive) {
                mvprintw(enemies[i].y, enemies[i].x, "V");
                enemies[i].y++;
                if (enemies[i].y >= HEIGHT) {
                    enemies[i].x = rand() % WIDTH;
                    enemies[i].y = 0;
                }
            }
        }

        // 碰撞检测
        for (int i = 0; i < ENEMY_NUM; i++) {
            if (enemies[i].alive && bullet.active &&
                bullet.x == enemies[i].x && bullet.y == enemies[i].y) {
                enemies[i].alive = 0;
                bullet.active = 0;
                score++;
            }
            // 敌机撞到飞机
            if (enemies[i].alive && enemies[i].x == plane_x && enemies[i].y == plane_y) {
                endwin();
                printf("Game Over! Score: %d\n", score);
                return 0;
            }
        }

        // 重新生成被打掉的敌机
        for (int i = 0; i < ENEMY_NUM; i++) {
            if (!enemies[i].alive) {
                enemies[i].x = rand() % WIDTH;
                enemies[i].y = 0;
                enemies[i].alive = 1;
            }
        }

        // 画分数
        mvprintw(0, 0, "Score: %d", score);

        // 处理输入
        int ch = getch();
        if (ch == KEY_LEFT && plane_x > 0) plane_x--;
        if (ch == KEY_RIGHT && plane_x < WIDTH - 1) plane_x++;
        if (ch == ' ' && !bullet.active) {
            bullet.x = plane_x;
            bullet.y = plane_y - 1;
            bullet.active = 1;
        }
        if (ch == 'q') break;

        refresh();
        usleep(50000);
    }

    endwin();
    return 0;
}
