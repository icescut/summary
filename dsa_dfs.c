/**
 * 数的全排列，使用深度搜索实现
 * gcc -o exe\dfs.exe dfs.c
 */
#include <stdio.h>

#define ASIZE 10

void dfs(int);

int main() {
    //输入全排列的个数
    scanf("%d", &n);
    dfs(1);
    return 0;
}

void dfs(int step) {
    int i;
    //前面n个都已放好，完成一次排列
    if(step == n + 1) {
        for(i = 1; i <= n; ++i)
            printf("%d ", a[i]);
        printf("\n");
    }
    //依次尝试
    for(i = 1; i<= n; ++i) {
        //如果没被使用
        if(book[i] == 0) {
            //标记为使用
            a[step] = i;
            book[i] = 1;
            //尝试下一步
            dfs(step + 1);
            //恢复状态，尝试下一个数
            book[i] = 0;
        }
    }
    return;
}
