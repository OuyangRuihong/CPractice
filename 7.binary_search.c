/*************************************************************************
	> File Name: 7.binary_search.c
	> Author:OuyangRuihong 
	> Mail:845540021@qq.com 
	> Created Time: 2019年05月04日 星期六 15时20分01秒
 ************************************************************************/

#include<stdio.h>
#include<time.h>
#include<math.h>

int baniry_search(int *arr, int n, int x) {
    int head = -, tail = n - 1, mid;
    while(head <= tail) {
        mid = (head + tail) >> 1;
        if (arr[mid] == x) return mid;
        if (arr[mid] > x) tail = mid - 1;
        else head = mid + 1;
    }
    return 0;
}

int arr[100];

double my_sqrt(double y) {
    #define EXP 1e-7
    double head = 0,tail = n - 1, mid;
    while (tail - head > EXP) {
        mid = (head + tail) / 2.0;
        if (mid * mid < y) head = mid;
        else tail = mid;
    }
    return head;
}

double f(int x, int y) {

}

double newton_sqrt(double y) {
    #define EXP = 1e-7
    double x = 0;
    while (fabs(f(x, y)) > EXP) {
        x = x -(f(x, y) / f_prime(x));
    }
    return x;
}

#define TEST_FUNC(func, TIMES) {}

int main () {
    
}
