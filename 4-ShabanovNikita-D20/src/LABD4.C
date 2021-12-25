//
//  main.c
//  PR4
//
//  Created by Никита Шабанов on 25.12.2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct QueneNode
{
    int NodeNumber;
} QueneNode;

typedef struct
{
    QueneNode **NodeArray;
    int NodeCount;
    int ReqSum;
    int SumArrayCount;
    QueneNode **SumArray;
} Quene;

Quene SystemQuene;

void SwapNodes(QueneNode **arr, int a, int b)
{
    QueneNode *t = arr[a];
    arr[a] = arr[b];
    arr[b] = t;
}

void QueneReset(int IndexStart, int IndexEnd)
{
    int i;
    
    for (i = IndexStart; i < IndexEnd; i++)
    {
        SystemQuene.SumArray[i] = NULL;
    }
}
  
int QuickPartition (QueneNode **arr, int beginning, int end)
{
    QueneNode *temp = arr[end];
    int i = (beginning - 1);
  
    for (int j = beginning; j <= end - 1; j++)
    {
        if (arr[j]->NodeNumber > temp->NodeNumber)
        {
            i++;
            SwapNodes(arr, i, j);
        }
    }
    SwapNodes(arr, i + 1, end);
    return (i + 1);
}
  
void QueneSort(QueneNode **arr, int beginning, int end)
{
    int pi;
    
    if (beginning < end)
    {
        pi = QuickPartition(arr, beginning, end);
  
        QueneSort(arr, beginning, end - 1);
        QueneSort(arr, beginning + 1, end);
    }
}

QueneNode* InitNode(void)
{
    QueneNode *ptr;
    
    ptr = (QueneNode*)malloc(sizeof(QueneNode));
    if (ptr == NULL)
        return NULL;
    
    return ptr;
}

void QueneInit(void)
{
    int i, NodeNum, SumNum;
    char num[64];
    void *ptr;
    
    fgets(num, 64, stdin);
    SumNum = atoi(num);
    
    fgets(num, 64, stdin);
    NodeNum = atoi(num);
    
    if (NodeNum <= 0 || SumNum <= 0)
    {
        SystemQuene.NodeArray = NULL;
        SystemQuene.SumArray  = NULL;
        return;
    }
    
    ptr = malloc(NodeNum * sizeof(QueneNode*));
    if (ptr == NULL)
        return;
    SystemQuene.NodeArray = (QueneNode**)ptr;
    
    ptr = malloc(NodeNum * sizeof(QueneNode*));
    if (ptr == NULL)
    {
        free(SystemQuene.NodeArray);
        return;
    }
    SystemQuene.SumArray = (QueneNode**)ptr;
    
    SystemQuene.SumArrayCount = 0;
    SystemQuene.NodeArray[0] = InitNode();
    SystemQuene.NodeCount = NodeNum;
    SystemQuene.ReqSum = SumNum;
    
    for (i = 1; i < NodeNum; i++)
    {
        SystemQuene.NodeArray[i] = InitNode();
    }
    
    for (i = 0; i < NodeNum; i++)
    {
        SystemQuene.SumArray[i] = InitNode();
    }
}

void QueneInput(void)
{
    int i, NodeNumber, NodeSign;
    char str[2048], str2[2048], *input;
    
    if (SystemQuene.NodeArray == NULL)
        return;
    
    NodeNumber = SystemQuene.NodeCount;
    
    gets(str);
    strcpy(str2, str);
    input = strtok(str2, " ");
    NodeSign = atoi(input);
    
    SystemQuene.NodeArray[0]->NodeNumber = NodeSign;
    
    for (i = 1; i < NodeNumber; i++)
    {
        input = strtok(NULL, " ");
        NodeSign = atoi(input);
        
        SystemQuene.NodeArray[i]->NodeNumber = NodeSign;
    }
}

void QueneFree(void)
{
    QueneNode *ptr;
    int i = 0;
    
    if (SystemQuene.NodeArray == NULL)
        return;
    
    ptr = SystemQuene.NodeArray[0];
    while (ptr != NULL && i < SystemQuene.NodeCount)
    {
        if (ptr != NULL)
            free(ptr);
        
        ptr = SystemQuene.NodeArray[i + 1];
        i++;
    }
    free(SystemQuene.NodeArray);
    if (SystemQuene.SumArray != NULL)
        free(SystemQuene.SumArray);
    
    SystemQuene.NodeArray = NULL;
    SystemQuene.SumArray = NULL;
}

void QueneDebug(void)
{
    QueneNode *ptr;
    int i = 0;
    
    if (SystemQuene.NodeArray == NULL)
        return;
    
    ptr = SystemQuene.NodeArray[0];
    while (ptr != NULL && i < SystemQuene.NodeCount)
    {
        printf("%i ", ptr->NodeNumber);
        
        ptr = SystemQuene.NodeArray[i + 1];
        i++;
    }
}

int FindSum(int RemainingSum, int ArrSumTag, int ArrNodTag)
{
    int i, NodeAmount, Num, a;
    
    if (RemainingSum < 0)
        return 0;
    if (RemainingSum == 0)
        return 1;
    
    NodeAmount = SystemQuene.NodeCount;
    
    for (i = ArrNodTag; i < NodeAmount; i++)
    {
        Num = SystemQuene.NodeArray[i]->NodeNumber;
        SystemQuene.SumArray[ArrSumTag] = SystemQuene.NodeArray[i];
        SystemQuene.SumArrayCount++;
        a = FindSum(RemainingSum - Num, ArrSumTag + 1, i + 1);
        if (a == 0)
        {
            QueneReset(ArrSumTag + 1, SystemQuene.SumArrayCount);
            SystemQuene.SumArrayCount = ArrSumTag;
        }
        else if (a == 1)
            return 1;
    }
    return 0;
}

void DisplayQuene(void)
{
    int i;
    
    printf("\n");
    for (i = 0; i < SystemQuene.SumArrayCount; i++)
        printf("%i ", SystemQuene.SumArray[i]->NodeNumber);
    printf("\n");
}

int main(int argc, const char * argv[]) {
    int a;
    QueneInit();
    QueneInput();
    
    /* Sorting is implemented to make counting of the sum of the elements start from the largest number. I have not checked whether it will actually make the algorithm work faster, but this is my way. */
    QueneSort(SystemQuene.NodeArray, 0, SystemQuene.NodeCount - 1);
    /* QueneDebug(); */
    a = FindSum(SystemQuene.ReqSum, 0, 0);
    
    if (a == 0)
        printf("%i \n", a);
    else if (a == 1)
        DisplayQuene();
    
    QueneFree();
    
    return 0;
}
