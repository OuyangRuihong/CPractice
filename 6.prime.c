/*************************************************************************
	> File Name: 6.prime.c
	> Author:OuyangRuihong 
	> Mail:845540021@qq.com 
	> Created Time: 2019年05月04日 星期六 14时36分39秒
 ************************************************************************/

#include<stdio.h>
#define MAX_N 10000

int prime[MAX_N + 5] = {0};

int main() {
    for (int i = 0; i * i < MAX_N; i++) {
        if (prime[i]) continue;
        for (int j = i * i; j <= MAX_N; j += i) {
            prime[j] = 1;
        } 
    }
    for (int i = 2; i <= MAX_N; i++) {
        if (prime[i]) continue;
        prime[++prime[0]] = i;
    }
    int n;
    while (~scanf("%d",&n)) {
        printf("%d\n", prime[n]);
    }
    return 0;
}
