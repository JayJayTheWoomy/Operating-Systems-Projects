CS 3013 – Operating Systems Projects

This repository contains four C-based projects completed for CS 3013 (Operating Systems) at WPI. 
Each project explores foundational OS topics including process control, synchronization, memory management, and scheduling policies.

---

## 📁 Project 1: The Process Party

**Focus:** Process creation, hierarchy, and execution control

This project includes five small programs:
- `prolific.c`: Spawns 8–13 children, each with random exit codes and wait times.
- `generation.c`: Recursively creates child processes to simulate process lineage.
- `explorer.c`: Navigates various system directories, spawning `ls -tr` subprocesses.
- `slug.c`: Simulates a slow-running process with randomized behavior.
- `slugrace.c`: Launches a race between four parallel `slug` processes, tracking real-time progress.

Concepts: `fork()`, `execvp()`, `waitpid()`, process trees, timing with `clock_gettime()`, random behavior seeded from `seed.txt`.

---

## 📁 Project 2: Synchronization and Concurrency (WGP)

**Focus:** Thread synchronization using semaphores or mutexes/condition variables

Simulates a multi-sport facility (Worcester Gompei Park) with:
- 36 baseball players (18 required to play)
- 44 football players (22 required)
- 60 rugby players (any even number ≤ 30 allowed)

Constraints:
- Only one sport type may use the field at a time.
- Threads must not busy-wait.
- Starvation prevention and fairness between sports.

Concepts: `pthread`, `mutex`, `cond`, or POSIX `semaphore` APIs; random delays; race conditions and fairness management.

---

## 📁 Project 3: Virtual Memory Manager

**Focus:** Paging and swapping in a simulated memory environment

Two-part project:
1. **Part 1** – Implements per-process page tables, address translation, and memory operations (`map`, `store`, `load`) in a simulated 64-byte physical memory.
2. **Part 2** – Adds swapping to disk when memory is full. Swapped pages are stored in a file, and page tables must track in-memory vs. disk state.

Concepts: Page tables, virtual-to-physical address translation, memory protection bits, swapping/eviction strategies, disk I/O simulation.

---

## 📁 Project 4: Scheduling Policies

**Focus:** Simulation and evaluation of CPU scheduling algorithms

Implements three scheduling algorithms:
- **FIFO (First-In-First-Out)**
- **SJF (Shortest Job First)**
- **RR (Round Robin)**

Features:
- Simulates execution traces for each algorithm.
- Computes per-job and average metrics: response time, turnaround time, and wait time.
- Includes five novel workload designs to test edge cases and scheduling behavior.

Concepts: Job queues, linked list structures, CPU scheduling theory, metrics analysis, preemption in RR.

---

Compilation and Execution

Each project contains a `Makefile` to build the executables. Run `make` or `make clean` as appropriate.

---
Notes
- All projects are implemented in ANSI C without non-standard libraries.
- Designed to compile and run on plain Ubuntu 20.04 VM.
