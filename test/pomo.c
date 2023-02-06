#include "pomo.h"

char data[5][8] = {0};
int check = 0;
int pclient = 0;
int cnt = 0;

void get_pid(int sig, siginfo_t *info, void *context)
{
    (void)context;
    (void)sig;
    pclient = info->si_pid;
    if (info->si_signo == SIGUSR1)
        check = 1;
}

char *time2str(int min, int sec) {
    char *str = NULL;
    char smin[2];
    char ssec[2];

    my_itoa(min, smin, 10);
    str = my_strconc(smin, ":");
    my_itoa(sec, ssec, 10);
    str = my_strconc(str, ssec);
    return str;    
}

char **str2seq(char *str) {
    char **seq = NULL;
    int y = 0;
    int x = 0;

    seq = malloc(sizeof(char) * 5);
    while (*str) {
        seq[y] = malloc(sizeof(char) * 9);
        for (int i = 7; i >= 0; --i) {
            seq[y][x] = ( (*str & (1 << i)) ? '1' : '0' );
            x++;
        }
        seq[y][9] = '\0';
        y++;
        x = 0;
        str++; 
    }
    return seq;
}

void send_seq(char **seq) {
    if (seq[0][0] == '1' || seq[0][0] == '0')
        printf("hehehehe\n");
    /*
    for (int y = 0; y < 5; y++) {
        for (int x = 0; x < 8; x++) {
            if (seq[y][x] == '1')
                printf("askip");
                kill(pclient, SIGUSR1);
            if (seq[y][x] == '0')
                kill(pclient, SIGUSR2);
            usleep(100);
        }
    } 
    */
}

void act_start(void) {
    struct sigaction act;
    int min = 24;
    int sec = 59;

    act.sa_flags = SA_SIGINFO|SA_RESTART;
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
            send_seq(str2seq(time2str(min, sec)));
            check = 0;
        }
    }    
}

void handler_clock(int sig) {
    int y = 0;
    int x = 0;

    if (sig == SIGUSR1) {
        if (x == 8) {
            y++;
            x = 0;
        }
        data[y][x++] = '1';
        cnt++;
    } else if (sig == SIGUSR2) {
        if (x == 8) {
            y++;
            x = 0;
        }
        data[y][x++] = '0';
        cnt++;
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
    while (1) {
        if (cnt == 40) {
            printf("final kill !\n");
            break;
        }
    }
    for (int y = 0; y < 5; y++)
        puts(data[y]);
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
