#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>

#define HEAP_SIZE 10240

// Singly Linked List node representing a metadata block before each memory chunk
typedef struct Block {
    size_t size;        // Size of the usable data area
    bool is_free;       // True if the block is currently unallocated
    struct Block* next; // Pointer to the next memory block node
} Block;

static char heap_pool[HEAP_SIZE];
static Block* free_list = NULL;

// Initializes the entire memory pool as a single large free block
void init_allocator() {
    free_list = (Block*)heap_pool;
    free_list->size = HEAP_SIZE - sizeof(Block);
    free_list->is_free = true;
    free_list->next = NULL;
}

// First-Fit memory allocation engine
void* my_malloc(size_t size) {
    Block* curr = free_list;

    while (curr != NULL) {
        if (curr->is_free && curr->size >= size) {
            // Check if the block can be split into a smaller allocated chunk and a new free block
            if (curr->size >= size + sizeof(Block) + 4) {
                Block* new_block = (Block*)((char*)curr + sizeof(Block) + size);
                new_block->size = curr->size - size - sizeof(Block);
                new_block->is_free = true;
                new_block->next = curr->next;

                curr->size = size;
                curr->next = new_block;
            }
            
            curr->is_free = false;
            // Return pointer shifting past the metadata block header directly to usable data space
            return (void*)((char*)curr + sizeof(Block));
        }
        curr = curr->next;
    }

    printf("System Error: Out of Memory! Allocation failed for %zu bytes.\n", size);
    return NULL;
}

// Worst-Fit memory allocation engine
void* worst_fit_malloc(size_t size){
    Block* largest = NULL;
    // Largest refers to the largest available free block found in the list

    Block* curr = free_list;
    while(curr != NULL){
        if(curr->is_free && curr->size >= size){
            if(largest == NULL){
                largest = curr; // First valid block becomes largest, then compared in the else clause in the next iterations
            } else{
                if(curr->size > largest->size){
                    largest = curr;
                }
            }
        }
        curr = curr->next;
    }

    if(largest == NULL){
        printf("System Error: Out of Memory! Allocation failed for %zu bytes.\n", size);
        return NULL;
    }

    if(largest->size >= size + sizeof(Block) + 4){
        // Check if the block can be split into a smaller allocated chunk and a new free block
        Block* new_block = (Block*)((char*)largest + sizeof(Block) + size);
        new_block->size = largest->size - size - sizeof(Block);
        new_block->is_free = true;
        new_block->next = largest->next;

        largest->size = size;
        largest->next = new_block;
    }

    largest->is_free = false;
    return (void*)((char*)largest+ sizeof(Block));
}

// Releases memory and immediately triggers a coalescing sweep
void my_free(void* ptr) {
    if (ptr == NULL) return;

    // Shift back to look at the structural header data
    Block* curr = (Block*)((char*)ptr - sizeof(Block));
    curr->is_free = true;

    // Coalescing Sweep: Merges adjacent free blocks to remove fragmentation gaps
    Block* temp = free_list;
    while (temp != NULL) {
        if (temp->is_free && temp->next != NULL && temp->next->is_free) {
            temp->size += sizeof(Block) + temp->next->size;
            temp->next = temp->next->next;
            continue; // Re-evaluate current node if consecutive free blocks exist
        }
        temp = temp->next;
    }
}

// Traverses the linked list and displays the current allocation grid state
void print_heap_status() {
    Block* curr = free_list;
    printf("\n--- Current Heap Memory Block Map Overview ---\n");
    int block_idx = 0;
    while (curr != NULL) {
        printf("Block [%d] | Size: %4zu Bytes | Condition: %s | Address: %p\n",
               block_idx++, curr->size, curr->is_free ? "FREE" : "ALLOCATED", (void*)curr);
        curr = curr->next;
    }
    printf("-------------------------------------------------\n");
}

int main() {
    printf("Initializing Custom System Heap Arena Space...\n");
    init_allocator();
    print_heap_status();

    printf("\n[Action 1]: Allocating space for data array (Requesting 40 Bytes)...\n");
    int* data_array = (int*)my_malloc(10 * sizeof(int));
    print_heap_status();

    printf("\n[Action 2]: Requesting workspace logging buffer (Requesting 200 Bytes)...\n");
    char* log_buffer = (char*)worst_fit_malloc(200 * sizeof(char)); // Tried worst fit here
    print_heap_status();

    printf("\n[Action 3]: Freeing data array allocation node...\n");
    my_free(data_array);
    print_heap_status();

    printf("\n[Action 4]: Freeing log buffer (Triggers automated block consolidation)...\n");
    my_free(log_buffer);
    print_heap_status();

    return 0;
}