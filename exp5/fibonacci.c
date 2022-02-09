#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_N 90

// this data is shared by the threads.
long int fib[MAX_N];

// thread func to compute fibonacci.
void *runner(void *param) {
	int n = atoi(param);
	fib[0] = 0;
	fib[1] = 1;
	for (int i = 2; i < n; i++) {
		fib[i] = fib[i - 1] + fib[i - 2];
	}
	pthread_exit(0);
}

int main(int argc, char *argv[]) {

	pthread_t tid;
	pthread_attr_t attr;

	if (argc != 2 || atoi(argv[1]) <= 0) {
		fprintf(stderr, "Usage : ./a.out <Positive Integer Value <= 90>\n");
		return -1;
	}
	
	// init thread with default attributes.
	pthread_attr_init(&attr);

	// create the thread.
	pthread_create(&tid, &attr, runner, argv[1]);
	
	// parent thread will wait for the child thread to exit.
	pthread_join(tid, NULL);
	
	// parent thread will output the fib sequence computed by the child thread.
	printf("Fibonacci sequence : ");
	for (int i = 0; i < atoi(argv[1]); i++) {
		printf("%ld ", fib[i]);
	}
	printf("\n");

	return 0;
}
