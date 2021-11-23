//
//  main.c
//  LAB2
//
//  Created by Никита Шабанов on 15.11.2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct MemoryBloc
{
    int allocatedsize;
    struct MemoryBloc *Next;
    int flagsize; // This is supposed to be (-1) * allocatedsize if the memorybloc is empty, at the end of the struct after all the data. The field itself is never filled.
} MemoryBloc;

typedef struct
{
    struct MemoryBloc *HeadFreeBloc;
    struct MemoryBloc *LastBloc;
    struct MemoryBloc *BeginningBloc;
    int systemsize;
} SystMem;

SystMem SystemAllocated;

int memgetminimumsize(void)
{
    int memsize = sizeof(MemoryBloc);
    return memsize;
}

int memgetblocksize(void)
{
    int memsize = sizeof(MemoryBloc);
    return memsize;
}

int meminit(void *pMemory, int size)
{
    if (pMemory != NULL && size >= memgetminimumsize())
    {
        SystemAllocated.LastBloc = (MemoryBloc*)pMemory;
        SystemAllocated.BeginningBloc = SystemAllocated.LastBloc;
        SystemAllocated.HeadFreeBloc = SystemAllocated.LastBloc;
        SystemAllocated.systemsize = size;
        
        SystemAllocated.LastBloc->allocatedsize = size;
        SystemAllocated.LastBloc->Next = NULL;
        *(char*)((char*)SystemAllocated.LastBloc + size - sizeof(int)) = -1 * size;
        
        return 1;
    }
    
    return 0;
}

void* memalloc(int size)
{
    MemoryBloc *oldBlock;
    MemoryBloc *newBlock;
    MemoryBloc *tempptr, *tempptr2;
    int tempsize;
    
    size += memgetblocksize();
    
    if (SystemAllocated.HeadFreeBloc == NULL)
    {
        return (void*) NULL;
    }
    
    if (SystemAllocated.LastBloc == NULL && SystemAllocated.HeadFreeBloc != NULL)
    {
        SystemAllocated.LastBloc = SystemAllocated.HeadFreeBloc;
    }
    
    oldBlock = SystemAllocated.LastBloc;
    
    while (oldBlock != NULL)
    {
        tempsize = *((char*)oldBlock + oldBlock->allocatedsize - sizeof(int));
        
        if (size > oldBlock->allocatedsize && oldBlock->Next == NULL)
            return (void*) NULL;
        
        if (size > oldBlock->allocatedsize && oldBlock->Next != NULL)
            oldBlock = oldBlock->Next;
        else if (size <= oldBlock->allocatedsize && tempsize > 0)
        {
            if (oldBlock->Next != NULL)
                oldBlock = oldBlock->Next;
            else
                return (void*) NULL;
        }
        else if (size > oldBlock->allocatedsize - memgetblocksize())
        {
            if (SystemAllocated.HeadFreeBloc == oldBlock)
            {
                tempsize = oldBlock->allocatedsize;
                
                if (oldBlock->Next != NULL)
                    tempptr = oldBlock->Next;
                else
                    tempptr = NULL;
                
                memset(oldBlock, 0, tempsize);
                newBlock = (MemoryBloc*)((char*)oldBlock);
                newBlock->allocatedsize = size;
                newBlock->Next = NULL;
                *((char*)newBlock + size - sizeof(int)) = size;
                
                SystemAllocated.LastBloc = tempptr;
                SystemAllocated.HeadFreeBloc = tempptr;
                
                return (void*) newBlock;
            }
            else
            {
                tempptr = SystemAllocated.HeadFreeBloc;
                while (tempptr->Next != oldBlock)
                {
                    if (tempptr->Next == NULL)
                        return (void*) NULL;
                    else
                        tempptr = tempptr->Next;
                }
                
                tempptr->Next = oldBlock->Next;
                
                memset(oldBlock, 0, tempsize);
                newBlock = (MemoryBloc*)((char*)oldBlock);
                newBlock->allocatedsize = size;
                newBlock->Next = NULL;
                *((char*)newBlock + size - sizeof(int)) = size;
                
                SystemAllocated.LastBloc = tempptr;
                
                return (void*) newBlock;
            }
        }
        else
        {
            if (SystemAllocated.HeadFreeBloc == oldBlock)
            {
                tempsize = oldBlock->allocatedsize;
                
                if (oldBlock->Next != NULL)
                    tempptr = oldBlock->Next;
                else
                    tempptr = NULL;
                
                newBlock = (MemoryBloc*)((char*)oldBlock);
                memset(newBlock, 0, size);
                
                *((char*)newBlock + size - sizeof(int)) = size;
                newBlock->allocatedsize = size;
                newBlock->Next = NULL;
                
                oldBlock = (MemoryBloc*)((char*)oldBlock + size);
                *((char*)oldBlock + size - sizeof(int)) = -1 * tempsize + size;
                oldBlock->allocatedsize = tempsize - size;
                oldBlock->Next = tempptr;
                
                SystemAllocated.LastBloc = oldBlock;
                SystemAllocated.HeadFreeBloc = oldBlock;
                
                return (void*) newBlock;
            }
            else
            {
                tempptr = SystemAllocated.HeadFreeBloc;
                tempptr2 = oldBlock->Next;
                tempsize = oldBlock->allocatedsize;
                
                while (tempptr->Next != oldBlock)
                {
                    if (tempptr->Next == NULL)
                        return (void*) NULL;
                    else
                        tempptr = tempptr->Next;
                }
                
                newBlock = (MemoryBloc*)((char*)oldBlock);
                memset(newBlock, 0, size);
                
                *((char*)newBlock + size - sizeof(int)) = size;
                
                oldBlock = (MemoryBloc*)((char*)oldBlock + size);
                
                tempptr->Next = oldBlock;
                *((char*)oldBlock + tempsize - sizeof(int)) = tempsize + size;
                oldBlock->allocatedsize = tempsize - size;
                oldBlock->Next = tempptr2;
                
                newBlock->Next = NULL;
                oldBlock->allocatedsize = oldBlock->allocatedsize - size;
                
                SystemAllocated.LastBloc = oldBlock;
                tempptr->Next = oldBlock;
                
                return (void*) newBlock;
            }
        }
    }
    return (void*) NULL;
}

void memfree(void *p)
{
    MemoryBloc *oldBlock;
    MemoryBloc *tempptr, *tempptr2;
    MemoryBloc *address;
    
    int tempsize;
    
    if (p == NULL)
        return;
    else
        address = (MemoryBloc*)p;
    
    if (address < SystemAllocated.BeginningBloc || address > (MemoryBloc*)((char*)SystemAllocated.BeginningBloc + SystemAllocated.systemsize))
        return;
    else if (address == SystemAllocated.BeginningBloc)
    {
        tempsize = address->allocatedsize;
        
        memset(address, 0, tempsize);
        
        address->Next = SystemAllocated.HeadFreeBloc;
        address->allocatedsize = tempsize;
        SystemAllocated.HeadFreeBloc = address;
        *((char*)address + tempsize - sizeof(int)) = -1 * tempsize;
        
        tempsize = *((char*)address + tempsize);
        tempptr = (MemoryBloc*)((char*)address + tempsize);
        tempptr2 = (MemoryBloc*)((char*)SystemAllocated.BeginningBloc + SystemAllocated.systemsize);
        
        if (*((char*)tempptr + tempsize - sizeof(int)) < 0 && tempptr < tempptr2)
        {
            tempptr2 = address;
            while (tempptr2->Next != tempptr)
            {
                if (tempptr2->Next == NULL)
                    return;
                else
                    tempptr2 = tempptr2->Next;
            }
            
            tempptr2->Next = address;
            
            SystemAllocated.HeadFreeBloc = SystemAllocated.HeadFreeBloc->Next;
            address->Next = tempptr->Next;
            address->allocatedsize += tempptr->allocatedsize;
            
            memset(tempptr, 0, tempptr->allocatedsize);
            *((char*)address + address->allocatedsize - sizeof(int)) = -1 * address->allocatedsize;
        }
    }
    else
    {
        tempsize = address->allocatedsize;
        memset(address, 0, address->allocatedsize);
        *((char*)address - sizeof(int) + tempsize) = -1 * tempsize;
        address->allocatedsize = tempsize;
        
        tempsize = *((char*)address - sizeof(int));
        tempptr = (MemoryBloc*)((char*)address - sizeof(int));
        tempptr2 = (MemoryBloc*)((char*)address - tempsize);
        
        if (tempsize < 0 && tempptr2 >= SystemAllocated.BeginningBloc)
        {
            oldBlock = (MemoryBloc*)((char*)address + tempsize);
            tempsize = address->allocatedsize;
            
            memset(tempptr, 0, sizeof(int));
            oldBlock->allocatedsize += address->allocatedsize;
            memset(address, 0, tempsize);
            
            *((char*)address - sizeof(int) + tempsize) = -1 * oldBlock->allocatedsize;
            address = oldBlock;
        }
        
        oldBlock = (MemoryBloc*)((char*)address + address->allocatedsize);
        tempptr2 = (MemoryBloc*)((char*)SystemAllocated.BeginningBloc + SystemAllocated.systemsize);
        
        if (tempsize < 0 && oldBlock < tempptr2)
        {
            tempsize = *((char*)oldBlock + oldBlock->allocatedsize);
            tempptr2 = SystemAllocated.HeadFreeBloc;
            tempptr = SystemAllocated.HeadFreeBloc;
            while (tempptr->Next != address)
            {
                if (tempptr->Next == NULL)
                    return;
                else
                    tempptr = tempptr->Next;
            }
            
            while (tempptr2->Next != oldBlock)
            {
                if (tempptr2->Next == NULL)
                    return;
                else
                    tempptr2 = tempptr2->Next;
            }
            
            tempptr->Next = address->Next;
            tempptr2->Next = address;
            address->Next = oldBlock->Next;
            
            memset(oldBlock, 0, oldBlock->allocatedsize);
            memset((char*)address + address->allocatedsize - sizeof(int), 0, sizeof(int));
            address->allocatedsize += oldBlock->allocatedsize;
            *((char*)address - sizeof(int) + address->allocatedsize) = -1 * address->allocatedsize;
        }
        
        if (address->Next == NULL)
        {
            address->Next = SystemAllocated.HeadFreeBloc;
            SystemAllocated.HeadFreeBloc = address;
        }
    }
}

void memdone( void )
{
    MemoryBloc *ptr, *end, *beginning;
    int ptrsize, tempsize;
    
    ptr = SystemAllocated.BeginningBloc;
    beginning = SystemAllocated.BeginningBloc;
    end = (MemoryBloc*)((char*)SystemAllocated.BeginningBloc + SystemAllocated.systemsize);
    
    while (ptr != end)
    {
        ptrsize = *((char*)ptr + ptr->allocatedsize - sizeof(int));
        tempsize = ptr->allocatedsize;
        
        if (ptrsize > 0)
        {
            memfree((void*)ptr);
        }
        ptr = (MemoryBloc*)((char*)ptr + tempsize);
    }
    
    tempsize = *((char*)beginning + beginning->allocatedsize - sizeof(int));
    
    if (tempsize == -1 * SystemAllocated.systemsize)
        memset(beginning, 0, beginning->allocatedsize);
}

int main(int argc, const char * argv[]) {
    void *p1 = 0, *p2 = 0, *ptr;
    int memsize = memgetminimumsize() + memgetblocksize() + 2;
    ptr = malloc(memsize);
    meminit(ptr, memsize);
    p1 = memalloc(1); // Success!
    p2 = memalloc(1); // Success!
    memfree(p2);
    memfree(p1);
    memdone();
    free(ptr);
    return 0;
}
