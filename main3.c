#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

// Child process function
void child_process() {
    pid_t pid = getpid();
    pid_t ppid = getppid();
    int iterations;
    int remaining_iterations;
    int sleep_time;
    int i;

    // Seed random generator for this child
    srandom(time(NULL) ^ (pid << 2));

    iterations = 1 + (random() % 30); // 1-30 iterations
    remaining_iterations = iterations;

    while (remaining_iterations > 0) {
        sleep_time = 1 + (random() % 10); // 1-10 seconds
        printf("Child Pid: %d is going to sleep for %d seconds...\n", pid, sleep_time);
        sleep(sleep_time);
        printf("Child Pid: %d is awake! Parent is %d\n", pid, ppid);
        remaining_iterations--;
    }

    exit(0); // End child process
}

int main() {
    int num_children = 2;
    pid_t pid;
    int status;
    pid_t done_pid;
    int finished = 0;
    int i;

    // Launch children
    for (i = 0; i < num_children; i++) {
        pid = fork();
        if (pid < 0) {
            perror("Fork failed");
            exit(1);
        } else if (pid == 0) {
            child_process(); // Child executes here
        }
    }

    // Parent waits for all children
    while (finished < num_children) {
        done_pid = wait(&status);
        if (done_pid > 0) {
            if (WIFEXITED(status)) {
                printf("Child Pid: %d finished successfully with exit code %d\n", done_pid, WEXITSTATUS(status));
            } else {
                printf("Child Pid: %d terminated abnormally\n", done_pid);
            }
            finished++;
        }
    }

    printf("Parent Pid: %d: All children have completed.\n", getpid());
    return 0;
}
