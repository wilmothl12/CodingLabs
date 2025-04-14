// ECE3600 Operating Systems Project 2
// Logan Wilmoth
// This program creates two child processes and connects
// the standard output of one to the standard input of the other using a pipe.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc != 2 && argc != 3) {
        fprintf(stderr, "Usage: %s cmd1 cmd2\n", argv[0]);
        exit(1);
    }

    int fd[2]; // file descriptors for pipe

    if (pipe(fd) == -1) {
        perror("pipe failed");
        exit(1);
    }

    // First child
    int pid1 = fork();
    if (pid1 < 0) {
        perror("fork failed");
        exit(1);
    }

    if (pid1 == 0) { // Child 1 
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]); 
        close(fd[1]); 

        execlp(argv[1], argv[1], NULL); 
        perror("execlp failed for cmd1");
        exit(1);
    }

    // Second child
    int pid2 = fork();
    if (pid2 < 0) {
        perror("fork failed");
        exit(1);
    }

    if (pid2 == 0) { 
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        close(fd[1]);

        execlp(argv[2], argv[2], NULL);
        perror("execlp failed for cmd2"); 
        exit(1);
    }

    close(fd[0]);
    close(fd[1]);

    wait(NULL); // Wait for the first child
    wait(NULL); // Wait for the second child

    return 0;
}
