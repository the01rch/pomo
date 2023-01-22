#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#define SIZE 100

int seq[8] = {0};
int check = 0;
int pclient = 0;
int cnt = 0;

void handler_clock(int sig) {
    /*
    if (sig == SIGUSR1)
        seq[cnt++] = 1;
    else if (sig == SIGUSR2)
        seq[cnt++] = 0;
        */
}

void get_pid(int sig, siginfo_t *info, void *context)
{
    (void)context;
    (void)sig;
    pclient = info->si_pid;
    if (info->si_signo == SIGUSR1)
        check = 1;
}

void act_start(void) {
    struct sigaction act;
    int min = 24;
    int sec = 59;

    act.sa_flags = SA_SIGINFO;
    act.sa_sigaction = get_pid;
    sigaction(SIGUSR1, &act, NULL);
    while (1) {
        if (min == 0 && sec == 0)
            break;
        if (sec == 0) {
            sec = 59;
            min--;
        }
        usleep(1000000);
        sec--;
        if (check > 0) {
            printf("%d:%d\n", min, sec);
            kill(pclient, 10);
            check = 0;
        }
    }    
}

void act_clock(void) {
    FILE *output;
    char buf[SIZE];
    struct sigaction act;

    output = popen("pgrep -f 'pomo start'", "r");
    if (output == NULL) {
        printf("error!");
    } else {
        int count = 1;
        while (fgets(buf, SIZE-1, output) != NULL)
            count++;
    }
    pclose(output);
    kill(atoi(buf), 10);
    pclient = getpid();
    act.sa_flags = SA_RESTART;
    act.sa_handler = &handler_clock;
    sigaction(SIGUSR1, &act, NULL);
    sigaction(SIGUSR2, &act, NULL);
    pause();
    return;
}

void flag(char *act) {
    if (my_strcmp("start", act))
        act_start();
    else if (my_strcmp("clock", act))
        act_clock();
}

int main(int ac, char **av) {
    if (ac != 2)
        return 1;
    flag(av[1]);
    return 0;
}
