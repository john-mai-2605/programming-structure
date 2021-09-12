#include "sighandler.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

static void alarm_handler(int iSig) {
    signal(SIGQUIT, sigquit_handler);
    return ;
}

void sigquit_handler(int iSig) {

    signal(SIGALRM, alarm_handler);
    printf("\nType Ctrl-\\ again within 5 seconds to exit\n");
    alarm(5);
    signal(SIGQUIT, exit_handler);
    return;
}

void exit_handler(int iSig) {
    exit(0);
}

void sigchld_handler(int iSig) {
    pid_t pid;
    while ( (pid = waitpid(-1, NULL, WNOHANG)) > 0) {
        printf("Child %d terminate normally", pid);
    }
    fflush(stdout);
}
