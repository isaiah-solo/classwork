#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/wait.h>

extern char **getline();

int len(char** input) {
	int length = 0;
	for (int i = 0; input[i] != NULL; i++) {
		length = i + 1;
	}
	return length;
}

int isSpecial(char* text) {
	return (strcmp(text, ")") == 0 ||
		strcmp(text, "(") == 0 ||
		strcmp(text, ">") == 0 ||
		strcmp(text, "<") == 0 ||
		strcmp(text, "|") == 0 ||
		strcmp(text, "&") == 0 ||
		strcmp(text, ";") == 0 ||
		strcmp(text, "*") == 0);
}

int checkCd(char** input, char* command, int low, int high) {
	if (strcmp(command, "cd") == 0 && high - low == 2) {
		chdir(input[high - 1]);
		return 1;
	}
	return 0;
}

void checkExit(char* command) {	
	if (strcmp(command, "exit") == 0) {
		exit(0);
	}
}

int internalShell(char** input, int low, int high) {
	if (low < 0) low = 0;
	char* command = input[low];
	checkExit(command);
	return checkCd(input, command, low, high);
}

char** createSubArray(char** input, int low, int high) {
	char** command = malloc(high * sizeof(char*));
	memcpy(command, &input[low], (high - low) * sizeof(*input));
	command[high - low] = 0;
	return command;
}

void runCommand(char** command) {
	pid_t pid = fork();
	if (pid != 0) {
		int status;
		waitpid(pid, &status, 0);
	} else {
		execvp(command[0], command);
	}
}

void runRedirectOut(char** prevCommand, char** command) {
	int fd = open(command[0], O_WRONLY | O_CREAT, 0640);
	int saved = dup(1);

	pid_t pid = fork();
	if (pid != 0) {
		int status;
		waitpid(pid, &status, 0);
	} else {
		dup2(fd, 1);
		execvp(prevCommand[0], prevCommand);
	}
	dup2(saved, 1);
	close(fd);
}

void runRedirectIn(char** prevCommand, char** command) {
	int fd = open(command[0], O_RDONLY | O_CREAT, 0755);
	int saved = dup(0);

	pid_t pid = fork();
	if (pid != 0) {
		int status;
		waitpid(pid, &status, 0);
	} else {
		dup2(fd, 0);
		execvp(prevCommand[0], prevCommand);
	}
	dup2(saved, 0);
	close(fd);
}

void runPipe(char** prevCommand, char** command) {
	int fd[2];
	pipe(fd);
	int saved = dup(1);

	pid_t pid = fork();
	if (pid != 0) {
		int status;
		waitpid(pid, &status, 0);
	} else {
		close(fd[0]);
		dup2(fd[1], 1);
		execvp(prevCommand[0], prevCommand);
	}

	dup2(saved, 1);
	saved = dup(0);
	pid_t pid2 = fork();
	if (pid2 != 0) {
		int status2;
		waitpid(pid2, &status2, 0);
	} else {
		close(fd[1]);
		dup2(fd[0], 0);
		execvp(command[0], command);
	}
	dup2(saved, 0);
}

int main() {
	char** input;

	// Infinite shell loop
	while (1) {
		write(1, "$ ", 2);
		input = getline();
		int low = 0;
		char** prevCommand = NULL;
		char* prevSpecial = NULL;
		for (int i = 0; input[i] != NULL; ++i) {
			char* text = input[i];
			if (!isSpecial(text)) continue;

			if (prevCommand != NULL && prevSpecial != NULL) {
				if (internalShell(prevCommand, 0, len(prevCommand))) continue;	
				if (strcmp(text, ">") == 0) {
					//runRedirectOut(input, prevCommand, low, i);
				}
			}

			prevCommand = createSubArray(input, low, i);
			prevSpecial = text;
			low = i + 1;
		}

		int length = len(input);
		char** command = createSubArray(input, low, length);
		if (length == 0) continue;

		if (prevCommand != NULL && prevSpecial != NULL) {
			if (internalShell(command, 0, len(command)) || internalShell(prevCommand, 0, len(prevCommand))) continue;
			if (len(prevCommand) == 0 || len(command) == 0) continue;

			if (strcmp(prevSpecial, ">") == 0) {
				runRedirectOut(prevCommand, command);
			} else if (strcmp(prevSpecial, "<") == 0) {
				runRedirectIn(prevCommand, command);
			} else if (strcmp(prevSpecial, "|") == 0) {
				runPipe(prevCommand, command);
			}
		} else {
			if (internalShell(command, 0, len(command))) continue;
			runCommand(command);
		}
	}
	return 0;
}
