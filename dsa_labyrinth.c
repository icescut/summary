/**
 * 最短路径到达迷宫的某点
 * gcc -o exe\labyrinth.exe labyrinth.c
 */
#include <stdio.h>
#define ASIZE 51

//考虑使用limits头文件
int n, m, p, q, min = 9999999;
int a[ASIZE][ASIZE];
int book[ASIZE][ASIZE];

void dfs(int, int, int);

int main() {
    int i, j, startx, starty;
    //有多少行，列
    scanf("%d %d", &n, &m);
    //接受行列详细数据
    for(i = 1; i <= n; ++i)
        for(j =1; j <= m; ++j)
            scanf("%d", &a[i][j]);
    //输入开始点，结束点
    scanf("%d %d %d %d", &startx, &starty, &p, &q);
    book[startx][starty] = 1;
    dfs(startx, starty, 0);

    printf("The shortest steps:%d", min);
    return 0;
}

void dfs(int x, int u, int step) {
    int next[4][2] = {
    {0, 1},
    {1, 0},
    {0, -1},
    {-1, 0}
    };
    int tx, ty, k;
    if(x == p && y == q) {
        min = min > step ? step : min;
        return;
    }
    for(k = 0; k < 4; ++k) {
        tx = x + next[k][0];
        ty = y + next[k][1];
        //是否越界
        if(tx < 1 || tx > n || ty < 1 || ty > m)
            continue;
        //不是障碍物及没走过
        if(a[tx][ty] == 0 && book[tx][ty] == 0) {
            book[tx][ty] = 1;
            dfs(tx, ty, step + 1);
            book[tx][ty] = 0;
        } 
    }
    return;
}
