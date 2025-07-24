#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/types.h>

// Function to generate a random number between min and max (inclusive)
int RN(int min, int max) {
    return rand() % (max - min + 1) + min;
}

// Function for life Generation
void LifeSpan(int life, int parentPID) {
    if (life > 0) {
        // Spawn a child process
        pid_t childPid = fork();

        // Checking for errors
        if (childPid == -1) {
            perror("Error forking process");
            exit(EXIT_FAILURE);
        }

        if (childPid == 0) {
            // Child process

            // Open the solution_output.txt file for appending
            FILE *solutionFile = fopen("solution_output.txt", "a");
            if (solutionFile == NULL) {
                perror("Error opening solution_output.txt");
                exit(EXIT_FAILURE);
            }

            // Print parent process information
            fprintf(solutionFile, "[Parent, PID: %d]: I am waiting for PID %d to finish.\n", parentPID, getpid());

            // Print child process information
            fprintf(solutionFile, "\t[Child, PID: %d]: I was called with descendant count=%d. I'll have %d descendant(s).\n", getpid(), life, life - 1);

            // Decrement the life count
            life--;

            fclose(solutionFile);

            // Recursive call with the new life
            LifeSpan(life, parentPID);

            // Child process exits
            exit(0);
        } else {

            // Open the solution_output.txt file for appending
            FILE *solutionFile = fopen("solution_output.txt", "a");
            if (solutionFile == NULL) {
                perror("Error opening solution_output.txt");
                exit(EXIT_FAILURE);
            }

            // Wait for the child process to exit
            int status;
            waitpid(childPid, &status, 0);

            // Print final information
            fprintf(solutionFile, "[Parent, PID: %d]: Child %d finished with status code %d. It’s now my turn to exit.\n", getpid(), childPid, WEXITSTATUS(status));

            fclose(solutionFile);

            // Exit with the child's exit code plus 1
            exit(WEXITSTATUS(status) + 1);
        }
    } else {
        // If the life is 0 or negative, exit.
        exit(0);
    }
}

int main() {
    // Read the random seed value from seed.txt
    FILE *seedFile = fopen("seed.txt", "r");
    if (seedFile == NULL) {
        perror("Error opening seed.txt");
        exit(EXIT_FAILURE);
    }

    int seed;
    fscanf(seedFile, "%d", &seed);
    fclose(seedFile);

    // Seed the srand() function with the read seed value
    srand(seed);

    // Pick a random number between 5 and 12 for life
    int life = RN(5, 12);

    // Open the solution_output.txt file for appending
    FILE *solutionFile = fopen("solution_output.txt", "a");
    if (solutionFile == NULL) {
        perror("Error opening solution_output.txt");
        exit(EXIT_FAILURE);
    }

    // Print the initial information
    fprintf(solutionFile, "shepard@normandy:~/cs3013/processes> ./generation\n");
    fprintf(solutionFile, "Read seed value: %d\n\n", seed);
    fprintf(solutionFile, "Read seed value (converted to integer): %d\n", seed);
    fprintf(solutionFile, "Random Descendant Count: %d\n", life);
    fprintf(solutionFile, "Time to meet the kids/grandkids/great grand kids/...\n");

    fclose(solutionFile);

    // Call the LifeSpan function with the initial life and parent PID
    LifeSpan(life, getpid());

    return 0;
}
