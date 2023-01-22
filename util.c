#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

bool my_strcmp(char *s1, char *s2) {
    for (int i = 0; s1[i] != '\0'; i++) {
        if (s2[i] != s1[i]) {
            return false;   
        }
    }
    return true;
}

void my_swap(char *c1, char *c2) {
    char tmp;

    tmp = *c1;
    *c1 = *c2;
    *c2 = tmp;
}

void strrev(char *str) {
    int size = strlen(str);
    int half = size / 2;
    int y = size - 1;
    
    for (int i = 0; i < half; i++) {
        my_swap(&str[i], &str[y]);
        y--;
    }
}

int itoa(int n, char *str, int base) {
    int rem = 0;
    int i = 0;
    bool neg = false;

    if (n == 0) {
        str[0] = '0';
        return 0;
    }
    if (n < 0) {
        n = -n;
        neg = true;
    }
    while (n != 0) { 
        rem = n % base;
        //str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        str[i++] = rem + '0';
        n = n / base;
    }
    if (neg == true)
       str[i] = '-'; 
    strrev(str);
    return 0; 
}
