/*************************************************************************
	> File Name: 8.array.c
	> Author:OuyangRuihong 
	> Mail:845540021@qq.com 
	> Created Time: 2019年05月04日 星期六 16时49分05秒
 ************************************************************************/

#include<stdio.h>

int fun_2(int **a) {//a 形参
    
}

int main() {
    int arr[100];
    printf("&arr[0] = &p  arr = %p\n", &arr[0], arr);
    printf("arr + 1 = %p  arr + 2 = %p\n",arr + 1, arr + 2);
    char *p = (char *)arr;
    printf("p + 1 = %p p + 2 = %p", p + 1, p + 2);
    return 0;
}

a[2][3]  *(*(a + 2) + 3)   *(a[2] + 3)  (a[2] + 3)[0]
