#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

/*
 * 			Creating this kind of Process Tree.
 *
 * 							A
 *
 *					B				C
 *
 *					D				E
 *
*/ 					


int main() {
    pid_t pid1, pid2, pid3, pid4;

    printf("Parent of all PID : %d\n", getpid());   // Say (A)

    pid1 = fork();

    if (pid1 == 0) {
        printf("Child PID : %d with parent PID : %d\n", getpid(), getppid());   // say (B)
        pid2 = fork();
        if (pid2 == 0) {
            printf("Child PID : %d with parent PID : %d\n", getpid(), getppid());  // say (D)
        }
		if(pid2 > 0) wait(NULL);
    }

    if (pid1 > 0) {
		wait(NULL);
        pid3 = fork();
        if (pid3 == 0) {
            printf("Child PID : %d with parent PID : %d\n", getpid(), getppid());  // say (C)
            pid4 = fork();
            if (pid4 == 0) {
                printf("Child PID : %d with parent PID : %d\n", getpid(), getppid());  // say (E).
            }
			if(pid4 > 0) wait(NULL);
        }
		if (pid3 > 0) wait(NULL);
    }

    return 0;
}

