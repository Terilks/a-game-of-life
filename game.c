#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ncurses.h>

#define WIDTH 80
#define HEIGHT 25

void LaunchGame(int canvas_before[HEIGHT][WIDTH], int canvas[HEIGHT][WIDTH]);
void InitSettings(int canvas_before[HEIGHT][WIDTH]) { srand(time(NULL)); 
        initscr(); // Initialize ncurses
    curs_set(0); // Hide the cursor
    noecho(); // Don't display user input
    timeout(0); // Non-blocking input
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
                if (scanf("%d", &canvas_before[i][j]) != 1) {
                    printf("Ошибка при чтении данных из stdin\n");
                    return 1;
                }
            }
        }
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

void DrawGame(int canvas_before[HEIGHT][WIDTH], int canvas[HEIGHT][WIDTH]) {
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

    usleep(100000);
    move(0, 0);
    CreateNextStep(canvas_before, canvas);
}

void LaunchGame(int canvas_before[HEIGHT][WIDTH], int canvas[HEIGHT][WIDTH]) {
    int ch;
    while (1) {
        DrawGame(canvas_before, canvas);
        CreateNextStep(canvas_before, canvas);

        ch = getch(); // Get user input
        if (ch == 'q') // Press 'q' to exit
            break;
        clear();
    }

    endwin(); // Clean up ncurses
}
