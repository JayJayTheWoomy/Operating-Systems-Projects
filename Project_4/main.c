#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struct to represent a job
struct job {
    int id;
    int length;
    int arrival_time;  // Added to keep track of arrival time for policy analysis
    struct job* next;
};

// Function declarations
void run_FIFO(struct job* job_list);
void run_SJF(struct job* job_list);
void run_RR(struct job* job_list, int time_slice);
void analyze_policy(struct job* job_list, const char* policy);
struct job* read_workload(const char* filename);

int main(int argc, char* argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <policy> <workload_file> <time_slice>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Parse command line arguments
    char* policy = argv[1];
    char* workload_file = argv[2];
    int time_slice = atoi(argv[3]);

    // Read workload from file
    struct job* job_list = read_workload(workload_file);

    // Run the appropriate scheduling policy
    if (strcmp(policy, "FIFO") == 0) {
        run_FIFO(job_list);
    } else if (strcmp(policy, "SJF") == 0) {
        run_SJF(job_list);
    } else if (strcmp(policy, "RR") == 0) {
        run_RR(job_list, time_slice);
    } else {
        fprintf(stderr, "Invalid scheduling policy: %s\n", policy);
        return EXIT_FAILURE;
    }

    // Analyze the performance of the policy
    analyze_policy(job_list, policy);

    // Clean up and free memory
    struct job* current = job_list;
    while (current != NULL) {
        struct job* next = current->next;
        free(current);
        current = next;
    }

    return EXIT_SUCCESS;
}

void run_FIFO(struct job* job_list) {
    printf("Execution trace with FIFO:\n");
    int total_time = 0;
    struct job* current = job_list;

    while (current != NULL) {
        printf("Job %d ran for: %d\n", current->id, current->length);
        total_time += current->length;
        current = current->next;
    }

    printf("End of execution with FIFO.\n");
}

void run_SJF(struct job* job_list) {
    printf("Execution trace with SJF:\n");
    int total_time = 0;

    // Sort the job list based on job length before running SJF
    // (Assuming bubble sort for simplicity, you can use more efficient sorting algorithms)
    struct job* i, * j;
    for (i = job_list; i != NULL; i = i->next) {
        for (j = i->next; j != NULL; j = j->next) {
            if (j->length < i->length) {
                // Swap the jobs
                int temp_id = i->id, temp_length = i->length;
                i->id = j->id;
                i->length = j->length;
                j->id = temp_id;
                j->length = temp_length;
            }
        }
    }

    // Run SJF
    while (job_list != NULL) {
        printf("Job %d ran for: %d\n", job_list->id, job_list->length);
        total_time += job_list->length;
        job_list = job_list->next;
    }

    printf("End of execution with SJF.\n");
}

void run_RR(struct job* job_list, int time_slice) {
    printf("Execution trace with RR:\n");
    int total_time = 0;
    struct job* current = job_list;

    while (current != NULL) {
        int run_time = (current->length < time_slice) ? current->length : time_slice;
        printf("Job %d ran for: %d\n", current->id, run_time);
        total_time += run_time;
        current->length -= run_time;

        if (current->length <= 0) {
            struct job* next = current->next;
            free(current);
            current = next;
        } else {
            current = current->next;
        }
    }

    printf("End of execution with RR.\n");
}

void analyze_policy(struct job* job_list, const char* policy) {
    printf("Begin analyzing %s:\n", policy);

    int total_response_time = 0;
    int total_turnaround_time = 0;
    int total_wait_time = 0;

    int current_time = 0;
    struct job* current = job_list;

    while (current != NULL) {
        printf("Job %d -- Response time: %d Turnaround: %d Wait: %d\n",
               current->id, current_time - current->arrival_time, current_time + current->length - current->arrival_time, current_time - current->arrival_time);

        total_response_time += current_time - current->arrival_time;
        total_turnaround_time += current_time + current->length - current->arrival_time;
        total_wait_time += current_time - current->arrival_time;

        current_time += current->length;
        current = current->next;
    }

    int num_jobs = current->id + 1;

    printf("Average -- Response: %.2f Turnaround %.2f Wait %.2f\n",
           (float)total_response_time / num_jobs,
           (float)total_turnaround_time / num_jobs,
           (float)total_wait_time / num_jobs);

    printf("End analyzing %s.\n", policy);
}

struct job* read_workload(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    struct job* head = NULL;
    struct job* tail = NULL;
    int id = 0;
    int arrival_time = 0;  // Initialize arrival time

    char line[256];
    while (fgets(line, sizeof(line), file) != NULL) {
        struct job* new_job = malloc(sizeof(struct job));
        if (new_job == NULL) {
            perror("Memory allocation error");
            exit(EXIT_FAILURE);
        }

        new_job->id = id++;
        new_job->length = atoi(line);
        new_job->arrival_time = arrival_time++;  // Set arrival time
        new_job->next = NULL;

        if (tail == NULL) {
            head = new_job;
            tail = new_job;
        } else {
            tail->next = new_job;
            tail = new_job;
        }
    }

    fclose(file);
    return head;
}
