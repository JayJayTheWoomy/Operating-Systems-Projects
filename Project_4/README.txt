# Scheduler Project

This project implements a simulated workload scheduler with three different scheduling policies: FIFO, SJF, and Round Robin. The scheduler takes a set of jobs from a workload file and simulates their execution based on the specified scheduling policy.

## Files:

- `main.c`: The main source code file containing the scheduler implementation.
- `Makefile`: Makefile for compiling the source code into an executable named `scheduler`.
- `README.txt`: This file providing information about the project.

## Compilation:

To compile the scheduler, use the following command:

```bash
make

Run the scheduler using the following format:
./scheduler <policy> <workload_file> <time_slice>


FIFO (First-In-First-Out):
The FIFO policy schedules jobs in the order of their arrival. Each job runs to completion without preemption.
Example:
./scheduler FIFO tests/1.in 0


SJF (Shortest Job First):
The SJF policy schedules jobs based on their runtime, selecting the job with the shortest runtime next. In case of ties, the job that arrived earlier is favored.
Example:
./scheduler SJF tests/2.in 0


Round Robin (RR):
The Round Robin policy schedules jobs in the order of arrival, allowing each job to run for a fixed time slice before moving to the next job. Preemption occurs if a job's remaining runtime exceeds the time slice.
Example:
./scheduler RR tests/3.in 5


To clean up compiled files, use the following command:
make clean
