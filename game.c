#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 80
#define HEIGHT 25

void LaunchGame(int canvas_before[HEIGHT][WIDTH], int canvas[HEIGHT][WIDTH]);

int main(int argc, char *argv[]) {
    int canvas_before[HEIGHT][WIDTH];
    int canvas[HEIGHT][WIDTH];
    
    LaunchGame(canvas_before, canvas);

    return 0;
}

void InitSettings() { srand(time(NULL)); }

void RandomCells(int canvas[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++) canvas[i][j] = rand() % 2;

    // for (int y = 0; y < HEIGHT; y++) {
    //         for (int x = 0; x < WIDTH; x++) {
    //             canvas[y][x] = 0;
    //         }
    //     }
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
                printf("#");
            else
                printf(" ");
        }
        printf("\n");
    }

    char c = getchar();
    printf("\e[1;1H\e[2J");
    CreateNextStep(canvas_before, canvas);
}



void LaunchGame(int canvas_before[HEIGHT][WIDTH], int canvas[HEIGHT][WIDTH]) {
    InitSettings();
    RandomCells(canvas_before);

    while (1) {
        DrawGame(canvas_before, canvas);
        
        
    }
}
