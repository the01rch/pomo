#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <stdlib.h>

int main(int ac, char **av) {
    printf("my pid is : %d", getpid());
    kill(atoi(av[1]), atoi(av[2]));
    return 0;
}
