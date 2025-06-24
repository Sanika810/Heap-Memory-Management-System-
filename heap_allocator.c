#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//structure to store the metadata of memory blocks
typedef struct BlockInfo 
{
    struct BlockInfo *next;
    int size;
    int allocated;//1 if allocated and 0 if free
} BlockInfo;

//heap of 10000 bytes of memory
char heapMemory[10000];

//freeblock having starting address of heap memory
BlockInfo *freeBlocks = (BlockInfo *)heapMemory;

//merging adjacent blocks to avoid fragmentation
void mergeBlocks(BlockInfo *previous)
{
    if (previous != NULL && previous->next != NULL)
    {
        BlockInfo *current = previous->next;
        //Check if current and current->next are Adjacent
        if (current->next != NULL && (void *)current + current->size + sizeof(BlockInfo) == current->next)//current->next is right after current, so we merge them.
        {
            current->size += (current->next)->size + sizeof(BlockInfo);
            current->next = current->next->next;
        }
        if ((void *)previous + previous->size + sizeof(BlockInfo) == current)
        {
            previous->next = current->next;
            previous->size += current->size + sizeof(BlockInfo);
        }
    }
}

//releasing allocated memory back to heap
void deallocateMemory(void *ptr)
{
    if (ptr == NULL)
    {
        printf("\nInvalid block");
    }
    else
    {
        BlockInfo *block = (BlockInfo *)ptr;
        block->allocated = 0;
        BlockInfo *temp = freeBlocks, *previous = NULL;
        while (temp != NULL && ptr > (void *)temp)
        {
            previous = temp;
            temp = temp->next;
        }
        //block can be inserted at first position
        //freeblock list pointer need to be updated
        if (freeBlocks == temp)
        {
            block->next = freeBlocks;
            freeBlocks = block;
        }
        //insert the block into freelist
        else
        {
            block->next = temp;
            previous->next = block;
        }
        //merge block if possible
        mergeBlocks(previous);
        printf("\nMemory deallocated.");
    }
}

//allocating memory in heap
void *allocateMemory(int blockSize)
{
    BlockInfo *ptr;
    //checking if the required memory size is available in the freeblock
    if (freeBlocks->next == NULL && (blockSize + sizeof(BlockInfo)) < freeBlocks->size)
    {
        ptr = freeBlocks;
        freeBlocks = (BlockInfo *)((void *)freeBlocks + blockSize + sizeof(BlockInfo));
        freeBlocks->size = ptr->size - sizeof(BlockInfo) - blockSize;
        freeBlocks->next = NULL;
        ptr->size = blockSize;
        ptr->next = NULL;
    }
    
    else 
    {
        BlockInfo *temp = freeBlocks, *previous = freeBlocks;
        while (temp != NULL && temp->size < (blockSize + sizeof(BlockInfo)))
        {
            previous = temp;
            temp = temp->next;
        }
        if (temp == NULL)
        {
            printf("No sufficient memory available.");
            return NULL;
        }
        ptr = temp;
        if (previous == temp)
        {
            freeBlocks = (BlockInfo *)((void *)temp + blockSize + sizeof(BlockInfo));
        }
        temp = (BlockInfo *)((void *)temp + blockSize + sizeof(BlockInfo));
        temp->size = ((BlockInfo *)ptr)->size - blockSize - sizeof(BlockInfo);
        temp->next = ((BlockInfo *)ptr)->next;
        ((BlockInfo *)ptr)->size = blockSize;
        ((BlockInfo *)ptr)->next = NULL;
        if (previous != temp)
        {
            previous->next = temp;
        }
    }
    ptr->allocated = 1;
    return (void *)ptr;
}

//printing the current status of heap
void printHeapStatus()
{
    printf("------------------------------------------------\n");
    BlockInfo *temp = (BlockInfo *)heapMemory;
    char status[10];
    while ((void *)temp < (void *)(heapMemory + 10000))
    {
        if(temp -> allocated)
        {
            strcpy(status , "Allocated");
        }
        else
        {
            strcpy(status , "Free");
        }
        printf("Block Address: %p | Block Size: %d bytes | Status: %s\n",
               temp, temp->size, status);
        temp = (BlockInfo *)((void *)temp + temp->size + sizeof(BlockInfo));
    }
    printf("------------------------------------------------\n");
}

int main()
{
    freeBlocks->next = NULL;
    freeBlocks->size = 10000 - sizeof(BlockInfo);
    freeBlocks->allocated = 0;
    
    int choice, size;
    void *ptr;
    while (choice != 4) 
    {
        printf("\nMenu:\n");
        printf("1. Allocate Memory\n");
        printf("2. Deallocate Memory\n");
        printf("3. Print Heap Status\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) 
        {
            case 1:
                printf("Enter size to allocate: ");
                scanf("%d", &size);
                ptr = allocateMemory(size);
                if (ptr) 
                {
                    printf("Memory allocated at address: %p\n", ptr);
                }
                break;

            case 2:
                printf("Enter address to deallocate: ");
                scanf("%p", &ptr);
                deallocateMemory(ptr);
                break;

            case 3:
                printHeapStatus();
                break;

            case 4:
                exit(0);

            default:
                printf("Invalid choice, please try again.\n");
        }
    }

    return 0;
}
