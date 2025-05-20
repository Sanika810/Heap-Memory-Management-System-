# Heap Memory Allocator in C

This project implements a simple memory allocator and deallocator in C, simulating how dynamic memory management might work within a fixed-size heap. It uses a custom-defined structure (`BlockInfo`) to manage metadata about each memory block.

## Features

* Manual memory allocation and deallocation
* Heap of fixed size (10,000 bytes)
* Merges adjacent free blocks to reduce fragmentation
* Displays real-time status of heap memory

## Data Structures

### `BlockInfo`

```c
typedef struct BlockInfo {
    struct BlockInfo *next;
    int size;
    int allocated; // 1 if allocated, 0 if free
} BlockInfo;
```

Each block in the heap includes metadata to track its size, allocation status, and the pointer to the next block.

## Memory Layout

* The heap is represented as a global array: `char heapMemory[10000];`
* The `freeBlocks` pointer keeps track of the free list.

## Functions

### `allocateMemory(int blockSize)`

Allocates a block of memory of the specified size. Splits blocks if necessary.

### `deallocateMemory(void *ptr)`

Deallocates a previously allocated memory block, inserts it back into the free list, and merges adjacent free blocks.

### `mergeBlocks(BlockInfo *previous)`

Helper function to merge adjacent free blocks to reduce fragmentation.

### `printHeapStatus()`

Prints the address, size, and status (allocated/free) of all memory blocks in the heap.

## Usage

Run the program and follow the on-screen menu:

```
Menu:
1. Allocate Memory
2. Deallocate Memory
3. Print Heap Status
4. Exit
```

### Example:

* Allocate 100 bytes: Choose 1, then enter 100
* View status: Choose 3
* Deallocate: Choose 2, then enter the address shown earlier

## Limitations

* Assumes correct addresses are entered for deallocation (no validation)
* Not thread-safe
* Only simulates heap behavior within a fixed-size array

## Compilation

Compile using any standard C compiler:

```bash
gcc -o heap_allocator heap_allocator.c
./heap_allocator
```

## Author

This code simulates a basic dynamic memory manager for educational purposes.
