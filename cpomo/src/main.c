#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

bool my_strcmp(char *s1, char *s2) {
    for (int i = 0; s1[i] != '\0'; i++) {
        if (s2[i] != s1[i]) {
            return false;   
        }
    }
    return true;
}

int main(int ac, char **av) {
    (void) ac;
    if (my_strcmp("start", av[1])) {
        int min = 0;
        int sec = 10;
        while (1) {
            if (min == 0 && sec == 0)
                break;
            if (sec == 0) {
                sec = 10;
                min--;
            }
            sleep(1);
            sec--;
            printf("%d:%d\n", min, sec);
        }
    }
    return 0;
}
