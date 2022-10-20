#include "../icl/proto.h"

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
            total = min * 60;
            total = total + sec;
            for (i = 0; total > 0; i++) {
                a[i] = total%2;
                total = total/2;
            }
            for (int y = 10;y > 0; y--) {
                if (a[y] == 0) 
                    kill(psender, 10);
                else if (a[y] == 1) 
                    kill(psender, 12);
                usleep(400);
            }
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

bool clock_flag(int pid) {
    int min = 0;
    int sec = 0;
    int check = 0;
    int a[10] = {0};
    int i = 0;
    char test[10] = {0};
    int bin = 0;
    int rem, base = 1, dec = 0;

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
        } else if (us2 > 0) {
            a[i++] = 1;
            us2 = 0;
        }
        if (i == 10)
            break;
    }
    for (int x = 0; x < 10; x++)
        test[x] = a[x] + '0';
    bin = atoi(test);
    while (bin > 0) {
        rem = bin % 10; 
        dec = dec + rem * base;
        bin = bin / 10;
        base = base * 2;
    }
    printf("dec = %d\n", dec);
    min = dec/60;
    sec = dec%60; 
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
