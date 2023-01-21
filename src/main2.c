#include <stdbool.h>
#include <signal.h>

bool my_strcmp(char *s1, char *s2) {
    for (int i = 0; s1[i] != '\0'; i++) {
        if (s2[i] != s1[i]) {
            return false;   
        }
    }
    return true;
}

void handler(int sig) {
    int i = 0;
    int seq[8];
    char c;

    if (sig == SIGUSR1)
        seq[i++] = 1;
    else if (sig == SIGUSR2)
        seq[i++] = 0;

}

void act_start(void) {
    while (1) {
        signal(SIGUSR1, handler);
        signal(SIGUSR2, handler);
        pause();
    }    
}

void act_clock(int pid) {
    return;
}

void flag(char *act) {
    if (my_strcmp("start", act))
        act_start();
    else if (my_strcmp("clock", act))
        act_clock(1212);
}

int main(int ac, char **av) {
    if (ac != 2)
        return 1;
    flag(av[1]);
    return 0;
}
