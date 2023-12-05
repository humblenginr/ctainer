#define _GNU_SOURCE    
#include <sched.h>

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


int child(void *args);
int spawn_bash(void);

int child(void *args)
{
    printf("pid as seen in the child: %lu\n", (unsigned long)getpid());
    // unmount all
    // chroot (bind mount/pivot root dance)
    // mount /proc (make /dev?)
    // remove capabilities? or switch user
    spawn_bash();
}

int spawn_bash(void)
{
    char *newargv[] = { "/bin/bash", NULL };

    execv("/bin/bash", newargv);
    perror("exec");
    exit(EXIT_FAILURE);
}

int main()
{
    int namespaces = CLONE_NEWUTS|CLONE_NEWPID|CLONE_NEWNS;
    pid_t p = clone(child, malloc(4096) + 4096, SIGCHLD|namespaces, NULL);
    if (p == -1) {
        perror("clone");
        exit(1);
    }
    printf("child pid: %lu\n", p);
    waitpid(p, NULL, 0);
    printf("Only now the process has exited \n");
    return 0;
}
