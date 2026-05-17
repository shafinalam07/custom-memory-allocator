# Custom Memory Allocator (Building my_malloc and my_free)

## What it does
Instead of just using the standard `malloc()` and `free()` functions from `stdlib.h`, I built my own memory allocator from scratch in C! 

This project sets up a pre-allocated 10KB array to simulate the system heap. It uses a Singly Linked List under the hood where each node acts as a "metadata header" that tracks the size and availability of the memory blocks right before the actual data space.

## Why I built this
I built this project to deeply understand:
1. How heap memory management actually works at a low level.
2. Pointer arithmetic and Type Casting (`void*` to `char*` transitions).
3. How to solve memory fragmentation problems manually.

## Features
- **First-Fit Strategy:** Scans the memory pool linearly and picks the first free block that is big enough for the requested size.
- **Block Splitting:** If a free block is way larger than what the user asked for, the allocator splits it into two parts—one allocated chunk and one new smaller free chunk—to prevent wasting space inside the block.
- **Automated Coalescing (Merging):** When you free a pointer, the system sweeps through the linked list and automatically merges adjacent free blocks together to prevent external fragmentation.

## How to run
1. Open your terminal inside this folder and compile the code:
   gcc allocator.c -o allocator

2. Run the compiled executable:
   ./allocator

## Tech used
- C Language
- GCC Compiler
- Data Structures (Singly Linked Lists & Dynamic Memory Concepts)

## Author
Shafin Alam
GitHub: shafinalam07
