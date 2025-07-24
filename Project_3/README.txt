
Memory Management Unit (MMU) Simulator

This program simulates a simple Memory Management Unit (MMU) that handles memory mapping, storing, and loading for multiple processes.

Compilation:
-------------
To compile the program, use the provided Makefile. Open a terminal and navigate to the directory containing the source code and Makefile. Then, run the following command:

```bash
make


The input format is as follows:
<processId>,<instructionType>,<virtualAddress>,<value>

    processId: Process identifier (integer).
    instructionType: Type of instruction ('m' for map, 's' for store, 'l' for load).
    virtualAddress: Virtual memory address (integer).
    value: Value associated with the instruction (integer).


Example input:
0,m,1024,1
1,s,2048,42
2,l,4096,0


To clean:
make clean


This code was created by Jason Luc and Dakota Wellerbrady

