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
        tempptr = oldBlock->Next;
        newBlock = (MemoryBloc*)((char*)oldBlock);
        
        if (size > truesize || tempsize > 0)
            oldBlock = oldBlock->Next;
        else if (size > truesize - memgetblocksize())
        {
            memset(oldBlock, 0, truesize);
            
            newBlock->allocatedsize = truesize;
            newBlock->Next = tempptr;
            
            SystemAllocated.LastBloc = newBlock;
            
            return (void*) newBlock;
        }
        else
        {
            memset(newBlock, 0, size);
            
            oldBlock = (MemoryBloc*)((char*)oldBlock + size);
            oldBlock->allocatedsize = -1 * (truesize - size);
            oldBlock->Next = tempptr;
            
            newBlock->Next = oldBlock;
            newBlock->allocatedsize = size;
            
            SystemAllocated.LastBloc = newBlock;
            return (void*) newBlock;
        }
    }
    SystemAllocated.LastBloc = SystemAllocated.HeadBloc;
    return memalloc(size - memgetblocksize());
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
    
    if (address->allocatedsize < 0 || address < SystemAllocated.HeadBloc || address >= (MemoryBloc*)((char*)SystemAllocated.HeadBloc + SystemAllocated.systemsize))
        return;
    
    tempsize = address->allocatedsize;
    truesize = abs(tempsize);
    tempptr = address->Next;
    tempptr2 = SystemAllocated.HeadBloc;
    
    memset(address, 0, truesize);
    address->Next = tempptr;
    address->allocatedsize = (-1) * truesize;
    
    if (address != SystemAllocated.HeadBloc)
    {
        while (tempptr2->Next != address)
        {
            if (tempptr2->Next == NULL)
                return;
            else
                tempptr2 = tempptr2->Next;
        }
        
        tempsize2 = tempptr2->allocatedsize;
        truesize2 = abs(tempsize2);
        
        if (tempsize2 < 0)
        {
            oldBlock = address->Next;
            
            memset(address, 0, truesize);
            tempptr2->allocatedsize += (-1) * truesize;
            tempptr2->Next = oldBlock;
            
            address = tempptr2;
        }
    }
    
    if (address->Next != NULL)
    {
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
}

void memdone( void )
{
    MemoryBloc *ptr, *end, *beginning, *tempptr;
    int ptrsize, tempsize, flag = 0;
    
    ptr = SystemAllocated.HeadBloc;
    beginning = SystemAllocated.HeadBloc;
    end = (MemoryBloc*)((char*)SystemAllocated.HeadBloc + SystemAllocated.systemsize);
    
    while (ptr != NULL)
    {
        flag = 0;
        ptrsize = ptr->allocatedsize;
        
        if (ptrsize > 0)
        {
            memfree((void*)ptr);
            flag = 1;
        }
        
        if (flag == 1)
        {
            ptr = SystemAllocated.HeadBloc;
        }
        else
            ptr = ptr->Next;
    }
    
    tempsize = beginning->allocatedsize;
    
    if (tempsize == -1 * SystemAllocated.systemsize)
        memset(beginning, 0, beginning->allocatedsize);
}

/*
int main(int argc, const char * argv[]) {
    void *p1 = 0, *p2 = 0, *p3 = 0, *p4 = 0, *p5 = 0, *p6 = 0,*ptr;
    int memsize = 150;
    ptr = malloc(memsize);
    meminit(ptr, memsize);
    
    p1 = memalloc(4);
    p2 = memalloc(12);
    p3 = memalloc(8);
    p4 = memalloc(7);
    p5 = memalloc(11);
    p6 = memalloc(5);
    
    memfree(p2);
    memfree(p5);
    
    memdone();
    free(ptr);
    return 0;
}
*/
