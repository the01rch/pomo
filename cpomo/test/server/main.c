#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

static volatile int signalPid = -1;
int test = 0;
int test2 = 0;

void    get_pid(int sig, siginfo_t *info, void *context)
{
    signalPid = info->si_pid;
    if (info->si_signo == 10)
        test++;
    if (info->si_signo == 12)
        test2++;
}

int main(int argc, char **argv)
{
    struct sigaction        sa;

    printf("PID: %d\n", getpid());       //display PID for kill()
    sa.sa_flags = SA_SIGINFO|SA_RESTART;
    sa.sa_sigaction = get_pid;
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);
    while (1) {
        if (test2 == 2)
            break;
        if (test > 0) {
            printf("PID of signal sender = %d\n", signalPid);
            test = 0;
        }
    }
   // while (1) {
   //     pause();
   //     printf("PID of signal sender = %d\n", signalPid);
   //     if (test > 0) {
   //         printf("PID of signal sender = %d\n", signalPid);
   //         test = 0;
   //     }
   //     if (test2 == 2)
   //         break;
   // }
}
