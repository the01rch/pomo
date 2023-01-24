#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define SIZE 100

char *my_strconc(char *s1, char *s2);
int my_itoa(int n, char *str, int base);
void my_strrev(char *str);
void my_swap(char *c1, char *c2);
bool my_strcmp(char *s1, char *s2);
