#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <math.h>
#include <stdlib.h>

int us1 = 0;
int us2 = 0;
int psender = 0;

bool my_strcmp(char *s1, char *s2) {
    for (int i = 0; s1[i] != '\0'; i++) {
        if (s2[i] != s1[i]) {
            return false;   
        }
    }
    return true;
}

void get_pid(int sig, siginfo_t *info, void *context) {
    (void)sig;
    (void)context;
    if (info->si_signo == SIGUSR1)
        us1++;
    if (info->si_signo == SIGUSR2)
        us2++;
    psender = info->si_pid;
}

void start_flag(void) {
    int min = 24;
    int sec = 59;
    int total = 0;
    int a[10] = {0};
    int i = 0;
    struct sigaction act;

	act.sa_flags = SA_SIGINFO|SA_RESTART;
    act.sa_sigaction = get_pid;
    if (sigaction(SIGUSR1, &act, NULL) == -1) {
        exit(1);
    }
    if (sigaction(SIGUSR2, &act, NULL) == -1) {
        exit(1);
    }
    printf("pid: %d\n", getpid());
    while (1) {
        if (us1 > 0) {
            printf("signal recu de clock\n");
            total = min * 60;
            total += sec;
            for (i = 0; total > 0; i++) {
                a[i] = total%2;
                total = total/2;
            }
            for (int y = 10;y > 0; y--) {
                if (a[y] == 0) 
                    kill(psender, 10);
                else if (a[y] == 1) 
                    kill(psender, 12);
                usleep(800);
            }
            printf("reponse envoyer pour clock\n");
            us1 = 0;
        }
        if (min == 0 && sec == 0)
            break;
        if (sec == 0) {
            sec = 59;
            min--;
        }
        sleep(1);
        sec--;
    }
}

bool send_signal(char *flag, int pid) {
    if (my_strcmp("pause", flag)) {
        if (kill(pid, 19) == -1)
            return false;
    } else if (my_strcmp("resume", flag)) {
        if (kill(pid, 18) == -1)
            return false;
    } else if (my_strcmp("stop", flag)) {
        if (kill(pid, 9) == -1)
            return false;
    }
    return true;
}

bool clock_flag(int pid) {
    //int min = 0;
    //int sec = 0;
    int check = 0;
    int a[10] = {0};
    int i = 0;
    char test[11] = {0};
    int bin = 0;

    struct sigaction act;

	act.sa_flags = SA_SIGINFO|SA_RESTART;
    act.sa_sigaction = get_pid;
    if (sigaction(SIGUSR1, &act, NULL) == -1) {
        exit(1);
    }
    if (sigaction(SIGUSR2, &act, NULL) == -1) {
        exit(1);
    }
    while (1) {
        if (check == 0) {
            kill(pid, 10);
            check = 1;
        }
        if (us1 > 0) {
            a[i++] = 0;
            us1 = 0;    
        }
        if (us2 > 0) {
            a[i++] = 1;
            us2 = 0;
        }
        if (i == 10)
            break;
    }
    for (int x = 0; x < 10; x++) {
        test[x] = a[x] + '0';
    }
    bin = atoi(test);
    int rem, base = 1, dec = 0;
    while (bin > 0) {
        rem = bin % 10; 
        dec = dec + rem * base;
        bin = bin / 10;
        base = base * 2;
    }
    printf("dec = %d\n", dec);
    int min = dec/60;
    int sec = dec%60; 
    printf("%d:%d\n", min, sec);
    return true;
}

bool gest_flag(char **av) {
    if (my_strcmp("start", av[1])) {
        start_flag();
        return true;
    } else if (my_strcmp("clock", av[1])) {
        clock_flag(atoi(av[2]));
        return true;
    } else if ((my_strcmp("pause", av[1])) 
            || (my_strcmp("resume", av[1])) 
            || (my_strcmp("stop", av[1]))) {
        send_signal(av[1], atoi(av[2]));
        return true;
    }
    return false;
}

int main(int ac, char **av) {
    if (ac == 1)
        return 1;
    else if (!gest_flag(av))
        return 1;
    return 0;
}
