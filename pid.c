#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    pid_t pid;

    pid = fork();

    if (pid == -1) {
        // Error handling for fork failure
        perror("fork");
        return 1;
    } else if (pid == 0) {
        // Child process
        printf("Child process: PID = %d\n", getpid());
    } else {
        // Parent process
        printf("Parent process: PID = %d\n", getpid());
        printf("Parent process: Child PID = %d\n", pid);
    }

    return 0;
}
