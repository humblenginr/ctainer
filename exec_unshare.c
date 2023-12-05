#define _GNU_SOURCE    
#include <sched.h>

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


int main()
{
    int namespaces = CLONE_NEWUTS|CLONE_NEWPID|CLONE_NEWNS|CLONE_NEWUSER;
    unshare(namespaces);
    int pid = fork();
    if (pid != 0) {
        printf("parent pid: %i\n", getpid());
        int status;
        waitpid(-1, &status, 0);
        return status;
    }
    printf("My pid: %i\n", getpid());
    char *args[]={"",NULL};
    sethostname("container",9);
    execv("/bin/bash",args);
    // now I have a process that is in it's own namespace
}
