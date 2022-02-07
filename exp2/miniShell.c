#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>


void parse(char *line, char **argv) {
	// remove the '\n' at the end added by the fgets().
	line[strcspn(line, "\n")] = 0;
	while (*line != '\0') {
		while (*line == ' ' || *line == '\t' || *line == '\n') *line++ = '\0';
		*argv++ = line;
		while (*line != '\0' && *line != ' ' && *line != '\t' && *line != '\n') line++;
	}
	*argv = '\0';
}


void execute(char **argv) {
	pid_t pid;
	int status;
	if ((pid = fork()) < 0) {
		// Unable to create the child process.
		printf("*** ERROR: forking child process failed\n");
		exit(1);
	}
	else if(pid == 0) {
		// execute the command for child process.
		if (execvp(*argv, argv) < 0) {
			printf("*** ERROR: exec failed\n");
			exit(1);
		}
	}
	else {
		// wait for child process to complete.
		while (wait(&status) != pid);
	}
}


void main(void) {
	char line[1024];
	char *argv[64];

	while (1) {
		printf("Shell -> ");
		fgets(line, sizeof(line), stdin);
		// if the command is empty.
		if (strcmp(line, "\n") == 0) continue;
		parse(line, argv);
		if (strcmp(argv[0], "exit") == 0) exit(0);
		execute(argv);
	}
}
