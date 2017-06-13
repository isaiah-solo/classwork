#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#define STDIN 0
#define STDOUT 1
#define BUFFER_SIZE 1

int main(int argc, char** argv) {

	// If arguments not provided
	while (argc < 2) {
		char buffer[BUFFER_SIZE] = {0};
		if (read(STDIN, buffer, BUFFER_SIZE) == 0) return(0);
		write(STDOUT, buffer, BUFFER_SIZE);
	}

	// If arguments provided
	for (int i = 1; i < argc; i++) {
		char* filename = argv[i];
		char buffer[BUFFER_SIZE] = {0};

		// Open file
		int file = open(filename, O_RDONLY);

		// If error in opening file
		if (file < 0) {
			perror("mycat");
			continue;
		}

		// Read from file
		int check = 0;
		while ((check = read(file, buffer, BUFFER_SIZE)) != 0) {

			// If error in reading file
			if (check < 0) {
				perror("mycat");
				return(1);
			}

			// Write to stdout
			write(STDOUT, buffer, BUFFER_SIZE);
		}

		// If error in closing file
		if (close(file) < 0) {
			perror("mycat");
			return(1);
		}
	}

	return(0);
}
