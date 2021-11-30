//
//  main.c
//  LAB2
//
//  Created by Никита Шабанов on 15.11.2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memallocator.h"

/* merge headfree, beginningbloc
   remove flagsize
   global lastbloc
   connect all elements
   
 */

typedef struct MemoryBloc
{
    int allocatedsize;
    struct MemoryBloc *Next;
} MemoryBloc;

typedef struct
{
    struct MemoryBloc *HeadBloc;
    struct MemoryBloc *LastBloc;
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
        SystemAllocated.HeadBloc = SystemAllocated.LastBloc;
        SystemAllocated.systemsize = size;
        
        SystemAllocated.LastBloc->allocatedsize = -1 * size;
        SystemAllocated.LastBloc->Next = NULL;
        
        return 1;
    }
    
    return 0;
}

void* memalloc(int size)
{
    MemoryBloc *oldBlock;
    MemoryBloc *newBlock;
    MemoryBloc *tempptr;
    int tempsize, truesize;
    
    size += memgetblocksize();
    
    if (SystemAllocated.HeadBloc == NULL || size < memgetblocksize())
    {
        return (void*) NULL;
    }
    
    if (SystemAllocated.LastBloc == NULL && SystemAllocated.HeadBloc != NULL)
    {
        SystemAllocated.LastBloc = SystemAllocated.HeadBloc;
    }
    
    oldBlock = SystemAllocated.LastBloc;
    
    while (oldBlock != NULL)
    {
        tempsize = oldBlock->allocatedsize;
        truesize = abs(tempsize);
        
        if (size > truesize && oldBlock->Next == NULL)
            return (void*) NULL;
        
        if (size > truesize && oldBlock->Next != NULL)
            oldBlock = oldBlock->Next;
        else if (size <= truesize && tempsize > 0)
        {
            if (oldBlock->Next != NULL)
                oldBlock = oldBlock->Next;
            else
                return (void*) NULL;
        }
        else if (size > truesize - memgetblocksize())
        {
            if (SystemAllocated.HeadBloc == oldBlock)
            {
                tempsize = oldBlock->allocatedsize;
                truesize = abs(tempsize);
                
                if (oldBlock->Next != NULL)
                    tempptr = oldBlock->Next;
                else
                    tempptr = NULL;
                
                memset(oldBlock, 0, truesize);
                newBlock = (MemoryBloc*)((char*)oldBlock);
                newBlock->allocatedsize = size;
                newBlock->Next = tempptr;
                
                SystemAllocated.LastBloc = newBlock;
                SystemAllocated.HeadBloc = newBlock;
                
                return (void*) newBlock;
            }
            else
            {
                tempptr = oldBlock->Next;
                
                memset(oldBlock, 0, truesize);
                newBlock = (MemoryBloc*)((char*)oldBlock);
                newBlock->allocatedsize = size;
                newBlock->Next = tempptr;
                
                SystemAllocated.LastBloc = newBlock;
                
                return (void*) newBlock;
            }
        }
        else
        {
            if (SystemAllocated.HeadBloc == oldBlock)
            {
                tempsize = oldBlock->allocatedsize;
                truesize = abs(tempsize);
                
                if (oldBlock->Next != NULL)
                    tempptr = oldBlock->Next;
                else
                    tempptr = NULL;
                
                newBlock = (MemoryBloc*)((char*)oldBlock);
                memset(newBlock, 0, size);
                
                newBlock->allocatedsize = size;
                
                oldBlock = (MemoryBloc*)((char*)oldBlock + size);
                oldBlock->allocatedsize = -1 * (truesize - size);
                oldBlock->Next = tempptr;
                
                newBlock->Next = oldBlock;
                
                SystemAllocated.LastBloc = newBlock;
                SystemAllocated.HeadBloc = newBlock;
                
                return (void*) newBlock;
            }
            else
            {
                tempptr = oldBlock->Next;
                tempsize = oldBlock->allocatedsize;
                truesize = abs(tempsize);
                
                newBlock = (MemoryBloc*)((char*)oldBlock);
                memset(newBlock, 0, size);
                
                oldBlock = (MemoryBloc*)((char*)oldBlock + size);
                
                oldBlock->allocatedsize = (-1) * (truesize - size);
                oldBlock->Next = tempptr;
                
                newBlock->Next = oldBlock;
                newBlock->allocatedsize = size;
                
                SystemAllocated.LastBloc = oldBlock;
                
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
    
    int tempsize, truesize, tempsize2, truesize2;
    
    if (p == NULL)
        return;
    else
        address = (MemoryBloc*)p;
    
    if (address->allocatedsize < 0)
        return;
    
    if (address < SystemAllocated.HeadBloc || address >= (MemoryBloc*)((char*)SystemAllocated.HeadBloc + SystemAllocated.systemsize))
        return;
    else if (address == SystemAllocated.HeadBloc)
    {
        tempsize = address->allocatedsize;
        truesize = abs(tempsize);
        tempptr = SystemAllocated.HeadBloc->Next;
        
        memset(address, 0, truesize);
        
        address->Next = tempptr;
        address->allocatedsize = (-1) * truesize;
        SystemAllocated.HeadBloc = address;
        
        tempptr = address->Next;
        tempsize = tempptr->allocatedsize;
        truesize = abs(tempsize);
        
        if (tempsize < 0)
        {
            tempptr2 = tempptr->Next;
            
            address->Next = tempptr2;
            address->allocatedsize += tempsize;
            
            memset(tempptr, 0, truesize);
        }
    }
    else
    {
        tempsize = address->allocatedsize;
        truesize = abs(tempsize);
        tempptr = SystemAllocated.HeadBloc;
        tempptr2 = address->Next;
        
        while (tempptr->Next != address)
        {
            if (tempptr->Next == NULL)
                return;
            else
                tempptr = tempptr->Next;
        }
        
        memset(address, 0, truesize);
        address->allocatedsize = (-1) * truesize;
        address->Next = tempptr2;
        
        tempsize2 = tempptr->allocatedsize;
        truesize2 = abs(tempsize2);
        
        if (tempsize2 < 0 && tempptr >= SystemAllocated.HeadBloc)
        {
            oldBlock = address->Next;
            
            memset(address, 0, truesize);
            tempptr->allocatedsize += (-1) * truesize;
            tempptr->Next = oldBlock;
            
            address = tempptr;
        }
        
        oldBlock = (MemoryBloc*)((char*)address + truesize);
        tempptr2 = (MemoryBloc*)((char*)SystemAllocated.HeadBloc + SystemAllocated.systemsize);
        
        if (oldBlock->allocatedsize < 0 && oldBlock < tempptr2)
        {
            tempsize = oldBlock->allocatedsize;
            truesize = abs(tempsize);
            tempptr = oldBlock->Next;
            
            address->Next = tempptr;
            
            memset(oldBlock, 0, truesize);
            address->allocatedsize += oldBlock->allocatedsize;
        }
    }
}

void memdone( void )
{
    MemoryBloc *ptr, *end, *beginning;
    int ptrsize, tempsize;
    
    ptr = SystemAllocated.HeadBloc;
    beginning = SystemAllocated.HeadBloc;
    end = (MemoryBloc*)((char*)SystemAllocated.HeadBloc + SystemAllocated.systemsize);
    
    while (ptr != NULL)
    {
        ptrsize = ptr->allocatedsize;
        
        if (ptrsize > 0)
        {
            memfree((void*)ptr);
        }
        
        ptr = ptr->Next;
    }
    
    tempsize = beginning->allocatedsize;
    
    if (tempsize == -1 * SystemAllocated.systemsize)
        memset(beginning, 0, beginning->allocatedsize);
}

/*
int main(int argc, const char * argv[]) {
    void *p1 = 0, *p2 = 0, *ptr;
    int memsize = memgetminimumsize() + memgetblocksize() + 2;
    ptr = malloc(memsize);
    meminit(ptr, memsize);
    p1 = memalloc(1); // Success!
    p2 = memalloc(1); // Success!
    memfree(p1);
    memfree(p2);
    memfree(p1);
    memdone();
    free(ptr);
    return 0;
}
*/
