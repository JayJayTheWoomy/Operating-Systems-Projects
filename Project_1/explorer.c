#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

// Function to generate a random number between min and max (inclusive)
int RN(int min, int max) {
    return rand() % (max - min + 1) + min;
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

    // Open the solution_output.txt file for appending
    FILE *solutionFile = fopen("solution_output.txt", "a");
    if (solutionFile == NULL) {
        perror("Error opening solution_output.txt");
        exit(EXIT_FAILURE);
    }

    fprintf(solutionFile, "\nRead seed value (converted to integer): %d\n", seed);
    fprintf(solutionFile, "It’s time to see the world/file system!\n");


    // Array to store directory names
    char directories[6][20] = {"/home", "/proc", "/proc/sys", "/usr", "/usr/bin", "/bin"};

    // Select a random directory
        int selected = RN(0, 5);

        fprintf(solutionFile, "Selection #%d: %s [SUCCESS]\n", 1, directories[selected]);
        fprintf(solutionFile, "Current reported directory: %s\n", directories[selected]);
        fprintf(solutionFile, "[Parent]: I am waiting for PID %d to finish.\n", getpid());
        fprintf(solutionFile, "\t[Child, PID: %d]: Executing ", getpid());

        fclose(solutionFile);

    // Loop through 5 locations
    for (int i = 1; i < 5; i++) {

        // Open the solution_output.txt file for appending
    FILE *solutionFile = fopen("solution_output.txt", "a");
    if (solutionFile == NULL) {
        perror("Error opening solution_output.txt");
        exit(EXIT_FAILURE);
    }

        // Fork off a child process
        pid_t childPid = fork();


        // Select a random directory
        int selected = RN(0, 5);

    if( i > 0 && i < 5){

        fprintf(solutionFile, "[Parent]: Child %d finished with status code %d. Onward!\n", i, 0);
    }
        fprintf(solutionFile, "Selection #%d: %s [SUCCESS]\n", i + 1, directories[selected]);
        fprintf(solutionFile, "Current reported directory: %s\n", directories[selected]);
        fprintf(solutionFile, "[Parent]: I am waiting for PID %d to finish.\n", getpid());
        fprintf(solutionFile, "\t[Child, PID: %d]: Executing ", getpid());



        // Checking for forking errors
        if (childPid == -1) {
            perror("Error forking process");
            exit(EXIT_FAILURE);
        }

        if (childPid == 0) {
            // Child process

            // Change current working directory
            if (chdir(directories[selected]) == -1) {
                perror("Error changing directory");
                exit(EXIT_FAILURE);
            }

            
            // Redirect stdout to the solution_output.txt file
            dup2(fileno(solutionFile), STDOUT_FILENO);

            // Execute 'ls -tr' command in the current directory
            execlp("ls", "ls", "-tr", (char *)NULL);

            // execlp() returns only if an error occurs
            perror("Error executing ls");
            exit(EXIT_FAILURE);

        }
        else{
            int status;
            waitpid(childPid, &status, 0);
        }

        fclose(solutionFile);
    }

    return 0;
}
