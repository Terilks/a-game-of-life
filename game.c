#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define WIDTH 80
#define HEIGHT 25

void LaunchGame(int canvas_before[HEIGHT][WIDTH], int canvas[HEIGHT][WIDTH]);
void InitSettings(int canvas_before[HEIGHT][WIDTH]);
void RandomCells(int canvas[HEIGHT][WIDTH]);

int main() {
    int canvas[HEIGHT][WIDTH];
    int canvas_future[HEIGHT][WIDTH];

    InitSettings(canvas);
    LaunchGame(canvas, canvas_future);

    return 0;
}

void ReadCanvas(int canvas_before[HEIGHT][WIDTH]) {
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
}

void RandomCells(int canvas[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++) canvas[i][j] = rand() % 2;
}

void InitSettings(int canvas_before[HEIGHT][WIDTH]) {
    srand(time(NULL));

    initscr();
    curs_set(0);
    noecho();
    timeout(0);
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    attron(COLOR_PAIR(1));
    keypad(stdscr, TRUE);

    ReadCanvas(canvas_before);
}

int GetCountNeighbor(int canvas[HEIGHT][WIDTH], int y, int x) {
    int count_lives = 0;

    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (dy == 0 && dx == 0) continue;
            if (canvas[(y + dy + HEIGHT) % HEIGHT][(x + dx + WIDTH) % WIDTH] == 1) count_lives++;
        }
    }
    return count_lives;
}

int IsWillLiveCeil(int canvas[HEIGHT][WIDTH], int y, int x) {
    int flag = 0;
    int count_lives = GetCountNeighbor(canvas, y, x);

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
            canvas_before[y][x] = IsWillLiveCeil(canvas, y, x);
        }
    }
}

void DrawGame(int canvas_before[HEIGHT][WIDTH], int canvas[HEIGHT][WIDTH], int *displaySpeed) {
    clear();
    attroff(COLOR_PAIR(1));
    printw("Speed: %d\n", *displaySpeed);
    attron(COLOR_PAIR(1));
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (canvas_before[y][x] == 1)
                printw("*");
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
    init_pair(1, COLOR_RED, COLOR_BLACK);
    printw("\n\n\n\n\n\n");
    DrawEndGame();
    refresh();
}

void LaunchGame(int canvas_before[HEIGHT][WIDTH], int canvas[HEIGHT][WIDTH]) {
    int ch;
    int speed = 2;
    int displaySpeed = 9;
    while (1) {
        DrawGame(canvas_before, canvas, &displaySpeed);
        CreateNextStep(canvas_before, canvas);
        napms(70 * speed);
        ch = getch();
        if (ch == 'q') break;

        if (ch == KEY_UP && speed > 1) {
            speed--;
        }

        if (ch == KEY_DOWN && speed < 10) {
            speed++;
        }

        speed == 10 ? displaySpeed = 1 : 0;
        speed == 9 ? displaySpeed = 2 : 0;
        speed == 8 ? displaySpeed = 3 : 0;
        speed == 7 ? displaySpeed = 4 : 0;
        speed == 6 ? displaySpeed = 5 : 0;
        speed == 5 ? displaySpeed = 6 : 0;
        speed == 4 ? displaySpeed = 7 : 0;
        speed == 3 ? displaySpeed = 8 : 0;
        speed == 2 ? displaySpeed = 9 : 0;
        speed == 1 ? displaySpeed = 10 : 0;
    }
    EndGame();
    napms(1500);
    endwin();
}
