#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
	int n;

	do {
		printf("Enter a positive number for collatz sequence : \n");
		scanf("%d", &n);
	}
	while (n <= 0);

	pid_t pid;

	pid = fork();

	if (pid == 0) {
		printf("child process is running..\n");
		while (1) {
			printf("%d ", n);
			if (n == 1) {
				break;
			}
			if (n % 2 == 0) n /= 2;
			else n = 3 * n + 1;
		}
		printf("\nchild process done executing.\n");
	}

	else if (pid > 0) {
		printf("Parent is waiting for the child to finish..\n");
		wait(NULL);
		printf("parent process is done executing.\n");
	}

	else {
		printf("Error occured!! Unable to create child process.\n");
	}
	return 0;
}