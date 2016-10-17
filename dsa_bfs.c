/**
 * 寻找到迷宫中某点的步数，使用广度优先
 */
#include <stdio.h>
struct node {
    int x;
    int y;
    int f; // 父节点在队列中的编号
    int s; //步数
};

int main() {
    struct node que[2501];  //地图大小不超过50*50
    int a[51][51] = {0};
    int book[51][51] = {0};
    int next[4][2] = {
    {0, 1},
    {1, 0},
    {0, -1},
    {-1, 0}
    };
    int head, tail;
    int i, j, k, n, m, startx, starty, p, q, tx, ty, flag;
    //地图的行数，列数
    scanf("%d %d", &n, &m);
    //输入地图的详细
    for(i = 1; i <= n; ++i)
        for(j = 1; j <= m; ++j)
            scanf("%d", &a[i][j]);
    //起点和终点
    scanf("%d %d %d %d", &startx, &starty, &p, &q);

    head = tail = 1;
    que[tail].x = startx;
    que[tail].y = starty;
    que[tail].f = 0;
    que[tail].s = 0;
    ++tail;
    book[startx][starty] = 1;
    flag = 0;
    while(head < tail) {
        //枚举四个方向
        for(k = 0; k < 4; ++k) {
            tx = que[head].x + next[k][0];
            ty = que[head].y + next[k][1];
            //是否越界
            if(tx <1 || tx > n || ty <1 || ty > m)
                continue;
             //没走过及不是障碍物
            if(a[tx][ty] == 0 && book[tx][ty] == 0) {
                book[tx][ty] = 1;
                que[tail].x = tx;
                que[tail].y = ty;
                que[tail].f = head;
                que[tail].s = que[head].s + 1;
                tail++;
            }
            if(tx == p && ty == q) {
                flag = 1;
                break;
            }
        }
        if(flag == 1)
            break;
        ++head;
    }
    printf("%d", que[tail - 1].s);
    return 0;
}
