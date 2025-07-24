#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t sluggers[4];

    int waiter = rand() % 5 + 2;
    // Fork off child processes
    for (int i = 0; i < 4; i++) {
        pid_t sluggy = fork();

		FILE *solutionFile = fopen("solution_output.txt", "a");

		fprintf(solutionFile, "[Parent]: I forked off child %d.\n", getpid());


        if (sluggy == -1) {
            perror("Error forking process");
            exit(EXIT_FAILURE);
        } 
        
        else if (sluggy == 0) { // Child process (slug doing stuff)

            char slugseed[11];
            sprintf(slugseed, "%d", i + 1);

            execl("./slug", "./slug", slugseed, NULL);

            // execlp() returns only if an error occurs
            perror("Error executing ./slug");
            exit(EXIT_FAILURE);
        } 
        
        else {

            sluggers[i] = sluggy;

            if(i == 3){
			fprintf(solutionFile, "The race is ongoing. The following children are still racing: %d %d %d %d\n", sluggers[0], sluggers[1], sluggers[2], sluggers[3]);
		    }
            
            fprintf(solutionFile, "\t[Child, PID: %d]: Executing ’./slug %d’ command...\n", getpid(), i+1);

        }

        fclose(solutionFile);
	
    }

    // Wait for all child processes to finish
    for (int i = 0; i < 4; i++) {
        waitpid(sluggers[i], NULL, 0);
        FILE *solutionFile = fopen("solution_output.txt", "a");
        fprintf(solutionFile,"Child %d has crossed the finish line! It took %d seconds", sluggers[i], waiter);
        fclose(solutionFile);
        
	}
    

    return 0;
}
