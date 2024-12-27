#include "../icl/pomo.h"

char data[41] = {0};
int check = 0;
int pclient = 0;
int cnt = 0;

void get_client_pid(int sig, siginfo_t *info, void *context)
{
    (void)context;
    (void)sig;
    pclient = info->si_pid;
    if (info->si_signo == SIGUSR1)
        check = 1;
}

char *bin2str(char *bin) {
    int len = strlen(bin);
    char *result = malloc(sizeof(char) * (len/8 + 1));
    char byte[9];
    int j = 0;

    byte[8] = '\0';
    for (int i = 0; i < len; i += 8) {
        strncpy(byte, &bin[i], 8);
        int char_value = 0;
        for (int k = 0; k < 8; k++)
            char_value = char_value * 2 + (byte[k] - '0');
        result[j++] = (char)char_value;
    }
    result[j] = '\0';
    return result;
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

    seq = malloc(sizeof(char *) * 5);
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
    for (int y = 0; y < 5; y++) {
        for (int x = 0; x < 8; x++) {
            if (seq[y][x] == '1') {
                kill(pclient, SIGUSR1);
                usleep(100);
            } else if (seq[y][x] == '0') {
                kill(pclient, SIGUSR2);
                usleep(100);
            }
            usleep(100);
        }
    } 
}

char *get_output_cmd(const char *cmd) {
    FILE *output;
    char *buf = malloc(sizeof(char) * SIZE);

    output = popen(cmd, "r");
    if (output == NULL) {
        printf("error!");
    } else {
        int count = 1;
        while (fgets(buf, SIZE-1, output) != NULL)
            count++;
    }
    pclose(output);
    return (buf);
}

static void act_start(void) {
    struct sigaction act;
    int min = 24;
    int sec = 59;
	char *cmd = get_output_cmd("pgrep -f 'pomo start' | wc -l");

	if (atoi(cmd) > 2)
		return (printf("Error :Pomo instance already running\n"), (void)0);
    act.sa_flags = SA_SIGINFO|SA_RESTART;
    act.sa_sigaction = get_client_pid;
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

static void handler_clock(int signum, siginfo_t *siginfo, void *unused) {
    (void)unused;
    (void)siginfo;
    if (signum == SIGUSR1) {
        data[cnt++] = '1';
    } else if (signum == SIGUSR2) {
        data[cnt++] = '0';
    }
}

static void act_clock(void) {
    struct sigaction act;
    char *buf = get_output_cmd("pgrep -f 'pomo start'");
    char *res = NULL;

	if (strlen(buf) == 0)
		return (printf("Error: pomo is not starting\n"), (void)0);
    kill(atoi(buf), 10);
    free(buf);
    pclient = getpid();
    act.sa_flags = SA_RESTART|SA_SIGINFO;
    act.sa_sigaction = handler_clock;
    sigaction(SIGUSR1, &act, NULL);
    sigaction(SIGUSR2, &act, NULL);
    while (1) {
        if (cnt == 40) {
            data[40] = '\0';
            break;
        }
    }
    res = bin2str(data);
    printf("%s\n", res);
    free(res);
    return;
}

int main(int ac, char **av) {
	char *buf = buf = strdup(get_output_cmd("pgrep -f 'pomo start'"));

    if (ac != 2)
        return 1;
    if (my_strcmp("start", av[1])) {
        act_start();
    } else if (my_strcmp("clock", av[1])) {
        act_clock();
    } else if (my_strcmp("pause", av[1])) {
        kill(atoi(buf), SIGSTOP);
    } else if (my_strcmp("resume", av[1])) {
        kill(atoi(buf), SIGCONT);
    } else if (my_strcmp("stop", av[1])) {
		system("kill -9 $(pgrep -f 'pomo start')");
    } else
        return 1;
    free(buf);
    return 0;
}
