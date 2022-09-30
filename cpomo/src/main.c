#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
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
            for (int i = 0; i < min; i++) {
                kill(psender, 10);
            }
            sleep(1);
            kill(psender, 12);
            sleep(1);
            for (int i = 0; i < sec; i++) {
                kill(psender, 10);
            }
            sleep(1);
            kill(psender, 12);
            sleep(1);
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
    //int check = 0;

    struct sigaction act;

	act.sa_flags = SA_SIGINFO|SA_RESTART;
    act.sa_sigaction = get_pid;
    if (sigaction(SIGUSR1, &act, NULL) == -1) {
        exit(1);
    }
    if (sigaction(SIGUSR2, &act, NULL) == -1) {
        exit(1);
    }
    if (kill(pid, 10) == -1)
        return false;
    while (1) {
        sleep(1);
        if (us2 == 1) {
            printf("us1 = %d\n", us1);
            us1 = 0;
        }
        if (us2 == 2) {
            printf("us1 = %d\n", us1);
            printf("bye !\n");
            break;
        }
        //if (us2 == 1 && check == 0) {
        //    min = us1;
        //    us1 = 0;
        //    check = 1;
        //}
        //if (us2 >= 2) {
        //    sec = us1;
        //    printf("%d:%d\n", min, sec);
        //    break;
        //}
    }
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
