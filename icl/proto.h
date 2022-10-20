#ifndef PROTO
# define PROTO

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

void get_pid(int sig, siginfo_t *info, void *context);
bool send_signal(char *flag, int pid);
bool my_strcmp(char *s1, char *s2);
bool gest_flag(char **av);

#endif
