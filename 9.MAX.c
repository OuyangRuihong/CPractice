/*************************************************************************
	> File Name: 9.MAX.c
	> Author:OuyangRuihong 
	> Mail:845540021@qq.com 
	> Created Time: 2019年05月04日 星期六 19时38分38秒
 ************************************************************************/

#include<stdio.h>
#define P(a, b) { \
    printf("%s = %d  expect = %d\n", #a, a, b); \
}

#define MAX(a, b) ((a) > (b) ? (a) : (b))

int main () {
    P(MAX(2, 3),3);
    P(5 + MAX(3, 4), 9);
    P(MAX(2,MAX(3, 4)), 4);
    P(MAX(2, 3 > 4 ? 3 : 4), 4);
    int a = 7;
    P(MAX(a++, 6), 7);
    P(a, 8);
    return 0;
}
