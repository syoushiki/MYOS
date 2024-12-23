#include <stdio.h>
#define N 6
#define M 5
int maze[N][M] = {
    {0, 1, 0, 0, 0},
    {0, 1, 0, 1, 0},
    {0, 0, 0, 0, 0},
    {0, 1, 1, 0, 0},
    {0, 0, 0, 1, 0},
    {0, 0, 0, 1, 0}
};
int MarkPath[N][M] = {0};
int Safelocation(int x, int y) {
    return (x >= 0 && x < N && y >= 0 && y < N && maze[x][y] == 0);
}
int solveMazeMark(int x, int y) {
    if (x == N - 1 && y == M - 1) {
        MarkPath[x][y] = 1;
        return 1;
    }
    if (Safelocation(x, y)) {
        MarkPath[x][y] = 1;
        if (solveMazeMark(x + 1, y) || solveMazeMark(x, y + 1)) {
            return 1;
        }
        MarkPath[x][y] = 0;
        return 0;
    }
    return 0;
}
void printSolution() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            printf("%d ", MarkPath[i][j]);
        }
        printf("\n");
    }
}
void solveMaze() {
    if (solveMazeMark(0, 0) == 0) {
        printf("No solution exists.\n");
    } else {
        printf("Solution:\n");
        printSolution();
    }
}
int main() {
    solveMaze();
    return 0;
}

