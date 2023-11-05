#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define WIDTH 80
#define HEIGHT 25

void LaunchGame(int canvas_before[HEIGHT][WIDTH], int canvas[HEIGHT][WIDTH]);
void InitSettings(int canvas_before[HEIGHT][WIDTH]) {
    srand(time(NULL));
    initscr();
    curs_set(0);
    noecho();
    timeout(0);
    keypad(stdscr, TRUE);

    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++) canvas_before[i][j] = 0;
}

void RandomCells(int canvas[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++) canvas[i][j] = rand() % 2;
}

int main() {
    int canvas_before[HEIGHT][WIDTH];
    int canvas[HEIGHT][WIDTH];

    InitSettings(canvas_before);

    if (isatty(fileno(stdin))) {
        RandomCells(canvas_before);
    } else {
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                scanf("%d", &canvas_before[i][j]);
            }
        }
    }

    if (freopen("/dev/tty", "r", stdin) != NULL) {
        TRUE;
    }

    LaunchGame(canvas_before, canvas);

    return 0;
}

int IsHaveNeighbor(int canvas[HEIGHT][WIDTH], int y, int x) {
    int flag = 0;
    int count_lives = 0;

    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (dy == 0 && dx == 0) continue;
            if (canvas[(y + dy + HEIGHT) % HEIGHT][(x + dx + WIDTH) % WIDTH] == 1) count_lives++;
        }
    }

    if (canvas[y][x] == 1) {
        if (count_lives == 2 || count_lives == 3)
            flag = 1;
        else
            flag = 0;
    } else {
        if (count_lives == 3)
            flag = 1;
        else
            flag = 0;
    }

    return flag;
}

void CreateNextStep(int canvas_before[HEIGHT][WIDTH], int canvas[HEIGHT][WIDTH]) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            canvas[y][x] = canvas_before[y][x];
        }
    }
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            canvas_before[y][x] = IsHaveNeighbor(canvas, y, x);
        }
    }
}

void DrawGame(int canvas_before[HEIGHT][WIDTH], int canvas[HEIGHT][WIDTH], int *displaySpeed) {
    clear();
    printw("Slowly Speed: %d\n", *displaySpeed);
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (canvas_before[y][x] == 1)
                printw("#");
            else
                printw(" ");
        }
        printw("\n");
    }
    refresh();
}

void DrawEndGame() {
    printw(" _____  ___ ___  ________   _____ _   _______ \n");
    printw("|  __ \\/ _ \\|  \\/  |  ___| |  ___| \\ | |  _  \\ \n");
    printw("| |  \\/ /_\\ \\ .  . | |__   | |__ |  \\| | | | |\n");
    printw("| | __|  _  | |\\/| |  __|  |  __|| . ` | | | |\n");
    printw("| |_\\ \\ | | | |  | | |___  | |___| |\\  | |/ / \n");
    printw(" \\____|_| |_|_|  |_|____/  \\____/\\_| \\_/___/  \n\n\n\n\n");
}

void EndGame() {
    clear();
    printw("\n\n\n\n\n\n");
    DrawEndGame();
    refresh();
}

void LaunchGame(int canvas_before[HEIGHT][WIDTH], int canvas[HEIGHT][WIDTH]) {
    int ch;
    int speed = 5;
    int displaySpeed = 1;
    while (1) {
        DrawGame(canvas_before, canvas, &displaySpeed);
        CreateNextStep(canvas_before, canvas);
        napms(70 * speed);
        ch = getch();
        if (ch == 'q')
            break;
        if (ch == KEY_UP && speed > 1) {
      speed--;
    }
    
    if (ch == KEY_DOWN && speed < 5) {
      speed++;
    }

    speed == 5 ? displaySpeed = 1 : 0;
    speed == 4 ? displaySpeed = 2 : 0;
    speed == 3 ? displaySpeed = 3 : 0;
    speed == 2 ? displaySpeed = 4 : 0;
    speed == 1 ? displaySpeed = 5 : 0;


    }
    EndGame();
    napms(1500);
    endwin();
}
