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
        
        SystemAllocated.LastBloc->allocatedsize = -1 * (size - memgetblocksize());
        SystemAllocated.LastBloc->Next = NULL;
        
        return 1;
    }
    
    SystemAllocated.LastBloc = NULL;
    SystemAllocated.HeadBloc = SystemAllocated.LastBloc;
    SystemAllocated.systemsize = 0;
    
    return 0;
}

void* memalloc(int size)
{
    MemoryBloc *oldBlock;
    MemoryBloc *newBlock;
    MemoryBloc *tempptr, *checkptr;
    int tempsize, truesize, flag = 0;
    
    if (SystemAllocated.HeadBloc == NULL || size <= 0)
        return NULL;
    
    if (SystemAllocated.LastBloc == NULL && SystemAllocated.HeadBloc != NULL)
        SystemAllocated.LastBloc = SystemAllocated.HeadBloc;
    
    if (SystemAllocated.LastBloc == SystemAllocated.HeadBloc)
        flag = 1;
    
    oldBlock = SystemAllocated.LastBloc;
    
    while (oldBlock != NULL)
    {
        tempsize = oldBlock->allocatedsize;
        truesize = abs(tempsize);
        tempptr = oldBlock->Next;
        newBlock = (MemoryBloc*)((char*)oldBlock);
        checkptr = (MemoryBloc*)((char*)oldBlock + size + memgetblocksize());
        
        if (size > truesize || tempsize >= 0)
            oldBlock = oldBlock->Next;
        else if (size + memgetblocksize() > truesize && size <= truesize)
        {
            memset(oldBlock, 0, truesize + memgetblocksize());
            
            newBlock->allocatedsize = truesize;
            newBlock->Next = tempptr;
            
            SystemAllocated.LastBloc = newBlock;
            
            return (void*)((char*)newBlock + memgetblocksize());
        }
        else if (checkptr <= (MemoryBloc*)((char*)SystemAllocated.HeadBloc + SystemAllocated.systemsize - memgetblocksize()))
        {
            memset(newBlock, 0, size + memgetblocksize());
            
            oldBlock = (MemoryBloc*)((char*)oldBlock + size + memgetblocksize());
            oldBlock->allocatedsize = -1 * (truesize - size - memgetblocksize());
            oldBlock->Next = tempptr;
            
            newBlock->Next = oldBlock;
            newBlock->allocatedsize = size;
            
            SystemAllocated.LastBloc = newBlock;
            return (void*)((char*)newBlock + memgetblocksize());
        }
        else
            oldBlock = NULL;
    }
    
    if (flag == 0)
    {
        SystemAllocated.LastBloc = SystemAllocated.HeadBloc;
        return memalloc(size);
    }
    
    return NULL;
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
        address = (MemoryBloc*)((char*)p - memgetblocksize());
    
    if (address < SystemAllocated.HeadBloc || address >= (MemoryBloc*)((char*)SystemAllocated.HeadBloc + SystemAllocated.systemsize))
        return;
    
    if (address->allocatedsize < 0)
        return;
    
    tempsize = address->allocatedsize;
    truesize = abs(tempsize);
    tempptr = address->Next;
    tempptr2 = SystemAllocated.HeadBloc;
    
    memset(address, 0, truesize + memgetblocksize());
    address->Next = tempptr;
    address->allocatedsize = (-1) * (truesize);
    
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
        
        if (tempsize2 <= 0)
        {
            oldBlock = address->Next;
            
            memset(address, 0, truesize + memgetblocksize());
            tempptr2->allocatedsize += (-1) * truesize - memgetblocksize();
            tempptr2->Next = oldBlock;
            
            address = tempptr2;
        }
    }
    
    if (address->Next != NULL)
    {
        tempptr = address->Next;
        tempsize = tempptr->allocatedsize;
        truesize = abs(tempsize);
        
        if (tempsize <= 0)
        {
            tempptr2 = tempptr->Next;
            
            address->Next = tempptr2;
            address->allocatedsize += tempsize - memgetblocksize();
            
            memset(tempptr, 0, truesize + memgetblocksize());
        }
    }
}

void memdone( void )
{
    MemoryBloc *ptr, *end, *beginning;
    int ptrsize, tempsize, flag = 0;
    
    ptr = SystemAllocated.HeadBloc;
    beginning = SystemAllocated.HeadBloc;
    end = (MemoryBloc*)((char*)SystemAllocated.HeadBloc + SystemAllocated.systemsize);
    
    if (SystemAllocated.HeadBloc)
        return;
    
    tempsize = beginning->allocatedsize;
    
    if (tempsize == -1 * SystemAllocated.systemsize + memgetblocksize())
        memset(beginning, 0, beginning->allocatedsize);
    
    SystemAllocated.HeadBloc = NULL;
}

int main(int argc, const char * argv[]) {
    void *p1 = 0, *p2 = 0, *p3 = 0, *p4 = 0, *p5 = 0, *p6 = 0, *ptr;
    int memsize = 250;
    ptr = malloc(memsize);
    meminit(ptr, memsize);
    
    p1 = memalloc(1);
    p2 = memalloc(3);
    p3 = memalloc(6);
    p4 = memalloc(4);
    p5 = memalloc(6);
    p6 = memalloc(9);
    
    memfree(p1);
    memfree(p2);
    memfree(p3);
    memfree(p4);
    memfree(p5);
    memfree(p6);
    
    memdone();
    free(ptr);
    return 0;
}
