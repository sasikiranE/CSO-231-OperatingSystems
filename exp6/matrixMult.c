#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define N 3

long int A[N][N];
long int B[N][N];
long int C[N][N];

void *runner(void *param) {
	long int row = (long int) param;
	for (long int col = 0; col < N; col++) {
		for (long int k = 0; k < N; k++) {
			C[row][col] += A[row][k] * B[k][col];
		}
	}
	pthread_exit(0);
}

void printMatrix(long int arr[][N], char c) {
	printf("Matrix %c :\n", c);
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			printf("%ld ", arr[i][j]);
		}
		printf("\n");
	}
	printf("----------\n");
}

int main() {
	for (long int i = 0; i < N; i++) { 
		for (long int j = 0; j < N; j++) {
			A[i][j] = rand() % 10;
			B[i][j] = rand() % 10;
		}
	}
	printMatrix(A, 'A');
	printMatrix(B, 'B');

	pthread_t threads[N];

	for (long int i = 0; i < N; i++) {
		pthread_create(&threads[i], NULL, runner, (void *) i);
	}

	for (long int i = 0; i < N; i++) {
		pthread_join(threads[i], NULL);
	}

	printMatrix(C, 'C');
	return 0;
}

