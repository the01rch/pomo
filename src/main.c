#include "../icl/proto.h"

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

int main(int ac, char **av) {
    if (ac == 1)
        return 1;
    else if (!gest_flag(av))
        return 1;
    return 0;
}
