# Custom Memory Allocator (Building my_malloc and my_free)

## What it does
Instead of just using the normal `malloc()` and `free()` functions that come with C, I built my own version from scratch! 

This program creates a fixed 10KB array to act as a mini fake heap memory. It uses a Singly Linked List to keep track of this memory. Every time you ask for space, a small "header" node is placed right before your data to store how big the block is and whether it is being used or free.

## Why I built this
I built this project to understand what happens behind the scenes when a computer allocates memory. It helped me learn:
1. **How the Heap Works:** Managing memory manually without relying on standard built-in shortcuts.
2. **Pointer Practice:** Getting comfortable with pointer arithmetic and changing data types (like `void*` and `char*`).
3. **Fixing Wasted Space:** Learning how to clean up memory clutter (fragmentation) so the program runs efficiently.

## Features
- **First-Fit Searching:** It loops through the memory list from the start and grabs the very first free spot that is big enough for your data.
- **Splitting Blocks:** If a free spot is way bigger than what you asked for, the program cuts it in half. One half goes to your data, and the other half stays free so space isn't wasted.
- **Merging Blocks (Coalescing):** When you free up a pointer, the program automatically looks left and right. If the neighboring blocks are also empty, it merges them back into one big clean block.

## How to run it
1. Open your terminal inside this folder and compile the code:
   gcc allocator.c -o allocator

2. Run the program:
   ./allocator

## Tech Used
- C Language
- GCC Compiler
- Core Concepts: Pointers, Singly Linked Lists, and Memory Allocation

## Author
Shafin Alam
GitHub: [shafinalam07](https://github.com/shafinalam07)
