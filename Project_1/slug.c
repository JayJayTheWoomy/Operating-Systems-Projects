#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>

void gachaPull(int fiftyFifty);

int main(int argc, char* argv[]) {
    // Debugging statements to print argc and argv
    printf("argc: %d\n", argc);
    for (int i = 0; i < argc; i++) {
        printf("argv[%d]: %s\n", i, argv[i]);
    }

    // Make it so that 1,2,3,4 will select either of the txt files.
    if (argc == 2) {
        FILE* f1;
        if (strcmp(argv[1], "1") == 0) {
            f1 = fopen("seed_slug_1.txt", "r");
        } else if (strcmp(argv[1], "2") == 0) {
            f1 = fopen("seed_slug_2.txt", "r");
        } else if (strcmp(argv[1], "3") == 0) {
            f1 = fopen("seed_slug_3.txt", "r");
        } else if (strcmp(argv[1], "4") == 0) {
            f1 = fopen("seed_slug_4.txt", "r");
        } else {
            exit(EXIT_FAILURE);
        }

        int seederBoi;
        fscanf(f1, "%d", &seederBoi);
        fclose(f1);
        srand(seederBoi);
        int waiter = rand() % 5 + 2;
        int fiftyFifty = rand() % 2;

        // Open the solution_output.txt file for appending
        FILE *solutionFile = fopen("solution_output.txt", "a");

        fprintf(solutionFile, "[Slug PID: %d] Read seed value: %d\n", getpid(), seederBoi);
        fprintf(solutionFile, "\n[Slug PID: %d] Read seed value (converted to integer): %d\n", getpid(), seederBoi);
        fprintf(solutionFile, "[Slug PID: %d] Delay time is %d seconds. Coin flip: %d\n", getpid(), waiter, fiftyFifty);
        fprintf(solutionFile, "[Slug PID: %d] I’ll get the job done. Eventually...\n", getpid());
		

        // Flush and close the file before forking
        fflush(solutionFile);

        // Create a child process
        pid_t childPid = fork();

        if (childPid == -1) {
            perror("Error forking process");
            exit(EXIT_FAILURE);
        }

        if (childPid == 0) {
            // Child process

            // Redirect stdout to the file using dup2
            dup2(fileno(solutionFile), STDOUT_FILENO);
            
            gachaPull(fiftyFifty);

            exit(0);       
        } 
            int status;
            waitpid(childPid, &status, 0);
            fclose(solutionFile);
    } 
    else {
        exit(EXIT_FAILURE);
    }

    exit(0);
}

void gachaPull(int fiftyFifty) {

    printf("[Slug PID: %d] Break time is over! I am running the ’id -group’ command.\n", getpid());

    // Dakota, you may need to change around the function with "execvp()"
    // or keep it as system().
    if (fiftyFifty == 0) {
        system("last -i -x");
    } else {
        system("id --group");
    }
}
