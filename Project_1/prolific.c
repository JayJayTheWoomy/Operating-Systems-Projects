#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

// Function to generate a random number between min and max (inclusive)
int RN(int min, int max) {
    return rand() % (max - min + 1) + min;
}

int main() {
    // Read the random seed value from seed.txt
    FILE *seedFile = fopen("seed.txt", "r"); // Opening seed.txt
    if (seedFile == NULL) {
        perror("Error opening seed.txt");
        exit(EXIT_FAILURE);
    }

	  // Open the solution_output.txt file for writing
    FILE *solutionFile = fopen("solution_output.txt", "a");
    if (solutionFile == NULL) {
        perror("Error opening solution_output.txt");
        exit(EXIT_FAILURE);
    }

    int seed; // Creating an int for the seed value.
    fscanf(seedFile, "%d", &seed); // Sets the value of the seed to the number in seed.txt
    fclose(seedFile); // Closing seed.txt

    // Seed the srand() function with the read seed value
    srand(seed);
    fprintf(solutionFile, "shepard@normandy:~/cs3013/processes> ./prolific\n");
    fprintf(solutionFile, "Read seed value: %d\n\n", seed);
	fprintf(solutionFile, "Read seed value (converted to integer): %d\n", seed);

    // Generate the number of children between 8 and 13
    int num_Children = RN(8, 13);

	fprintf(solutionFile, "Random Child Count: %d\nI'm feeling prolific!\n", num_Children);

	fclose(solutionFile);

    // Array to store random numbers for each child
    int randomNumbers[num_Children];

    // Generate random numbers for each child and store it in the array
    for (int i = 0; i < num_Children; i++) {
        randomNumbers[i] = RN(1, 100);
    }

    // Counter to keep track of child processes
    int counter = 0;

    // Close the standard output
    fclose(stdout);

    // Parent waits for all child processes to finish
    for (int i = 0; i < num_Children; i++) {
        int status;
        pid_t finishedChild = waitpid(-1, &status, 0);

		  // Open the solution_output.txt file for writing
		fopen("solution_output.txt", "a");
		if (solutionFile == NULL) {
			perror("Error opening solution_output.txt");
			exit(EXIT_FAILURE);
		}
		
		  // Fork off child processes
        pid_t childPid = fork();

        // Checking for forking errors
        if (childPid == -1) {
            perror("Error forking process");
            exit(EXIT_FAILURE);
        }

        if (childPid == 0) {
            // Child process

            // Get child process ID
            pid_t myPid = getpid();

            // Extract random value for the child from the array
            int myRandom = randomNumbers[counter];

            // Determine exit code and wait time
            int exitCode = ((myRandom % 50) + 1);
            int waitTime = ((myRandom % 3) + 1);

            // Redirect stdout to the solution_output.txt file
            dup2(fileno(solutionFile), STDOUT_FILENO);

            // Print child process information
            fprintf(solutionFile,"\t[Child,PID:%d]I am the child and I will wait %d secondsand exsit with code %d\n", i+1, waitTime, exitCode);

            sleep(waitTime);

			fprintf(solutionFile,"\t[Child,PID:%d]Now exiting...\n", i+1);
			if (WIFEXITED(status)) {
            // Print information about the child process to the file
            fprintf(solutionFile, "[Parent]:Child %d finished with status code %d. Onward!\n", i + 1, WEXITSTATUS(status));
        } else {
            fprintf(solutionFile, "Child process %d did not exit successfully\n", i + 1);
        }
		
            exit(exitCode);

			fclose(solutionFile);

        } else {
            // Parent process
            counter++;
        }

		  // Open the solution_output.txt file for writing
		fopen("\nsolution_output.txt", "a");
		if (solutionFile == NULL) {
			perror("Error opening solution_output.txt");
			exit(EXIT_FAILURE);
		}

		fprintf(solutionFile, "[Parent]:I am waiting for PID %d to finish\n",i + 1);


		 // Close the file
    fclose(solutionFile);

    }

}
