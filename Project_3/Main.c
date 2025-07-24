#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMORY_SIZE 64          // Size of physical memory
#define PAGE_SIZE 16            // Size of each page in memory
#define NUM_PROCESSES 4         // Number of processes in the system

// Physical memory and page table registers
unsigned char memory[MEMORY_SIZE];   // Physical memory array
int pageTableRegisters[NUM_PROCESSES] = { -1, -1, -1, -1 };  // Page table registers for each process

// Define a structure for page table entries
typedef struct {
    int frameNumber;  // Physical frame number where the page is stored
    int valid;        // Flag indicating whether the page is valid or not
    int writeable;    // Flag indicating whether the page is writeable or read-only
    int swapped;      // Flag indicating whether the page is swapped to disk
    int swapFileLocation; // Location in the swap file if swapped
} PageTableEntry;

// 2D array to store page tables for each process
PageTableEntry pageTables[NUM_PROCESSES][MEMORY_SIZE / PAGE_SIZE];

// Swap file
#define DISK_SWAP_FILE_PATH "./disk.txt"
FILE* swapFile;

// Function to open the swap file
void MMUOpenSwapFile() {
    swapFile = fopen(DISK_SWAP_FILE_PATH, "w+");
    if (swapFile == NULL) {
        perror("Error opening swap file");
        exit(EXIT_FAILURE);
    }
}

// Function to allocate a page table for a given process
void allocatePageTable(int processId) {
    if (pageTableRegisters[processId] == -1) {
        // Allocate a new page table if not already allocated
        int pageTableFrameNumber = processId;
        pageTableRegisters[processId] = pageTableFrameNumber;

        // Initialize page table entries for the process
        for (int i = 0; i < MEMORY_SIZE / PAGE_SIZE; i++) {
            pageTables[processId][i].valid = 0;
            pageTables[processId][i].writeable = 0;
            pageTables[processId][i].swapped = 0;
            pageTables[processId][i].swapFileLocation = -1;
        }
    }
}

// Function to translate a virtual address to a physical address
int translateAddress(int processId, int virtualAddress, int *writeable) {
    int virtualPageNumber = virtualAddress / PAGE_SIZE;
    int offset = virtualAddress % PAGE_SIZE;

    // Check if the page table is allocated for the process, allocate if not
    if (pageTableRegisters[processId] == -1) {
        allocatePageTable(processId);
    }

    // Check if the virtual page is valid in the page table
    if (pageTables[processId][virtualPageNumber].valid) {
        *writeable = pageTables[processId][virtualPageNumber].writeable;
        int frameNumber = pageTables[processId][virtualPageNumber].frameNumber;
        return frameNumber * PAGE_SIZE + offset;
    } else {
        // Return -1 if the virtual page is not valid
        *writeable = 0;
        return -1;
    }
}

// Function to handle page eviction (swap out to disk)
void evictPage(int processId, int virtualPageNumber) {
    int frameNumber = pageTables[processId][virtualPageNumber].frameNumber;

    // Write the page to the swap file
    fseek(swapFile, frameNumber * PAGE_SIZE, SEEK_SET);
    fwrite(&memory[frameNumber * PAGE_SIZE], PAGE_SIZE, 1, swapFile);

    // Update page table to indicate the page is swapped to disk
    pageTables[processId][virtualPageNumber].swapped = 1;
    pageTables[processId][virtualPageNumber].swapFileLocation = frameNumber;
    pageTables[processId][virtualPageNumber].valid = 0;
}

// Function to handle page loading (swap in from disk)
void loadPage(int processId, int virtualPageNumber) {
    int frameNumber = pageTables[processId][virtualPageNumber].frameNumber;

    // Read the page from the swap file
    fseek(swapFile, pageTables[processId][virtualPageNumber].swapFileLocation * PAGE_SIZE, SEEK_SET);
    fread(&memory[frameNumber * PAGE_SIZE], PAGE_SIZE, 1, swapFile);

    // Update page table to indicate the page is in memory
    pageTables[processId][virtualPageNumber].swapped = 0;
    pageTables[processId][virtualPageNumber].valid = 1;
}

// Function to handle a 'map' instruction
void handleMapInstruction(int processId, int virtualAddress, int value) {
    int virtualPageNumber = virtualAddress / PAGE_SIZE;

    // Check if the page table is allocated for the process, allocate if not
    if (pageTableRegisters[processId] == -1) {
        allocatePageTable(processId);
    }

    // Check if the virtual page is already mapped
    if (pageTables[processId][virtualPageNumber].valid) {
        printf("Warning: Page already mapped for process %d at virtual address %d\n", processId, virtualAddress);
    } else {
        // Find a free physical page
        int freePage = -1;
        for (int i = 0; i < MEMORY_SIZE / PAGE_SIZE; i++) {
            if (!pageTables[processId][i].valid) {
                freePage = i;
                break;
            }
        }

        if (freePage == -1) {
            // Evict a page using a simple round-robin eviction strategy
            freePage = processId % (MEMORY_SIZE / PAGE_SIZE);
            evictPage(processId, freePage);
        }

        // Map the virtual address to a physical frame
        int frameNumber = processId * (MEMORY_SIZE / PAGE_SIZE) + freePage;
        pageTables[processId][virtualPageNumber].frameNumber = frameNumber;
        pageTables[processId][virtualPageNumber].valid = 1;
        pageTables[processId][virtualPageNumber].writeable = value;
        printf("Mapped virtual address %d (page %d) into physical frame %d\n", virtualAddress, virtualPageNumber, frameNumber);
    }
}

// Function to handle a 'store' instruction
void handleStoreInstruction(int processId, int virtualAddress, int value) {
    int writeable;
    int physicalAddress = translateAddress(processId, virtualAddress, &writeable);

    // Check if the virtual address is valid
    if (physicalAddress != -1) {
        // Check if the page is writeable before storing the value
        if (writeable) {
            memory[physicalAddress] = (unsigned char)value;
            printf("Stored value %d at virtual address %d (physical address %d)\n", value, virtualAddress, physicalAddress);
        } else {
            printf("Warning: Attempted write to a read-only page at virtual address %d\n", virtualAddress);
        }
    } else {
        // Print an error message for an invalid virtual address
        printf("Error: Invalid virtual address %d for process %d\n", virtualAddress, processId);
    }
}

// Function to handle a 'load' instruction
void handleLoadInstruction(int processId, int virtualAddress, int value) {
    int writeable;
    int physicalAddress = translateAddress(processId, virtualAddress, &writeable);

    // Check if the virtual address is valid
    if (physicalAddress != -1) {
        // Load the value from the memory and print it
        int loadedValue = memory[physicalAddress];
        printf("The value %d is at virtual address %d (physical address %d)\n", loadedValue, virtualAddress, physicalAddress);
    } else {
        // Print an error message for an invalid virtual address
        printf("Error: Invalid virtual address %d for process %d\n", virtualAddress, processId);
    }
}

// Main function to execute instructions
int main() {
    char instructionType;
    int processId, virtualAddress, value;

    // Open the swap file
    MMUOpenSwapFile();

    // Read instructions until the end of input
    while (scanf("%d,%c,%d,%d", &processId, &instructionType, &virtualAddress, &value) == 4) {
        // Process each instruction type
        switch (instructionType) {
            case 'm':
                handleMapInstruction(processId, virtualAddress, value);
                break;
            case 's':
                handleStoreInstruction(processId, virtualAddress, value);
                break;
            case 'l':
                handleLoadInstruction(processId, virtualAddress, value);
                break;
            default:
                // Print an error message for an unknown instruction type
                printf("Error: Unknown instruction type %c\n", instructionType);
        }
    }

    // Close the swap file
    fclose(swapFile);

    return 0;
}
