//
//  main.c
//  LAB3E2
//
//  Created by Никита Шабанов on 22.12.2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mactime.h"

typedef struct TreeNode
{
    int NodeSign;
    int AdjCount;
    struct TreeNode **AdjacentNodes;
    int VisitedFlag;
    struct TreeNode *Next;
} TreeNode;

typedef struct
{
    struct TreeNode **NodeArray;
    int NodeCount;
    struct TreeNode *QueneHead;
} Quene;

Quene TreeSystem;

void SwapNodes(TreeNode **arr, int a, int b)
{
    TreeNode *t = arr[a];
    arr[a] = arr[b];
    arr[b] = t;
}
  
int QuickPartition (TreeNode **arr, int beginning, int end)
{
    TreeNode *temp = arr[end];
    int i = (beginning - 1);
  
    for (int j = beginning; j <= end - 1; j++)
    {
        if (arr[j]->NodeSign < temp->NodeSign)
        {
            i++;
            SwapNodes(arr, i, j);
        }
    }
    SwapNodes(arr, i + 1, end);
    return (i + 1);
}
  
void AdjSort(TreeNode **arr, int beginning, int end)
{
    int pi;
    
    if (beginning < end)
    {
        pi = QuickPartition(arr, beginning, end);
  
        AdjSort(arr, beginning, end - 1);
        AdjSort(arr, beginning + 1, end);
    }
}
  

TreeNode* InitNode(int NodeSign)
{
    TreeNode *ptr;
    if (NodeSign < 0)
        return NULL;
    
    ptr = (TreeNode*)malloc(sizeof(TreeNode));
    if (ptr == NULL)
        return NULL;
    ptr->NodeSign = NodeSign;
    ptr->VisitedFlag = 0;
    ptr->AdjCount = 0;
    ptr->AdjacentNodes = malloc(0);
    ptr->Next = NULL;
    
    return ptr;
}

void TreeInit(int NodeNum)
{
    int i;
    if (NodeNum == 0)
    {
        TreeSystem.NodeArray = NULL;
        return;
    }
    
    TreeSystem.NodeArray = (TreeNode**)malloc(NodeNum * sizeof(TreeNode*));
    TreeSystem.NodeArray[0] = InitNode(0);
    TreeSystem.NodeCount = NodeNum;
    
    for (i = 1; i < NodeNum; i++)
    {
        TreeSystem.NodeArray[i] = InitNode(i);
    }
}

void AddAdjacent(TreeNode *AdjNode, int NodeRecepient)
{
    TreeNode *ptr = TreeSystem.NodeArray[NodeRecepient];
    
    if (ptr == NULL)
        return;
    
    ptr->AdjacentNodes = (TreeNode**)realloc(ptr->AdjacentNodes, sizeof(TreeNode*) * (ptr->AdjCount + 1));
    ptr->AdjacentNodes[ptr->AdjCount] = AdjNode;
    ptr->AdjCount++;
}

void TreeInput(void)
{
    int i, a, NodeNumber, NodeSign, AdjCount;
    char str[256], str2[256], *input, resetptr[2] = "aa";
    
    if (TreeSystem.NodeArray == NULL)
        return;
    
    NodeNumber = TreeSystem.NodeCount;
    
    for (i = 0; i < NodeNumber; i++)
    {
        gets(str);
        strcpy(str2, str);
        input = strtok(str2, " ");
        NodeSign = atoi(input);
        AdjCount = -1;
        
        while (input != NULL)
        {
            AdjCount++;
            input = strtok(NULL, " ");
        }
        TreeSystem.NodeArray[i]->AdjCount += AdjCount;
        TreeSystem.NodeArray[i]->AdjacentNodes = (TreeNode**)realloc(TreeSystem.NodeArray[i]->AdjacentNodes, sizeof(TreeNode*) * TreeSystem.NodeArray[i]->AdjCount);
        
        strcpy(str2, str);
        strtok(resetptr, " ");
        input = strtok(str2, " ");
        a = TreeSystem.NodeArray[i]->AdjCount - AdjCount;
        
        while (a < TreeSystem.NodeArray[i]->AdjCount)
        {
            input = strtok(NULL, " ");
            if (atoi(input) > i)
                AddAdjacent(TreeSystem.NodeArray[i], atoi(input));
            TreeSystem.NodeArray[i]->AdjacentNodes[a] = TreeSystem.NodeArray[atoi(input)];
            a++;
        }
    }
}

void TreeInputForTest(char *fileaddress)
{
    int i, a, NodeNumber, NodeSign, AdjCount;
    char str[1024], str2[1024], *input, resetptr[2] = "aa";
    FILE *F;
 
    if (TreeSystem.NodeArray == NULL)
        return;
    
    F = fopen(fileaddress, "r");
    if (F == NULL)
        return;
    
    fgets(str, 1024, F);
    NodeNumber = TreeSystem.NodeCount;
    
    for (i = 0; i < NodeNumber; i++)
    {
        fgets(str, 1024, F);
        strcpy(str2, str);
        input = strtok(str2, " ");
        NodeSign = atoi(input);
        AdjCount = -1;
        
        while (input != NULL)
        {
            AdjCount++;
            input = strtok(NULL, " ");
        }
        TreeSystem.NodeArray[i]->AdjCount += AdjCount;
        TreeSystem.NodeArray[i]->AdjacentNodes = (TreeNode**)realloc(TreeSystem.NodeArray[i]->AdjacentNodes, sizeof(TreeNode*) * TreeSystem.NodeArray[i]->AdjCount);
        
        strcpy(str2, str);
        strtok(resetptr, " ");
        input = strtok(str2, " ");
        a = TreeSystem.NodeArray[i]->AdjCount - AdjCount;
        
        while (a < TreeSystem.NodeArray[i]->AdjCount)
        {
            input = strtok(NULL, " ");
            if (atoi(input) > i)
                AddAdjacent(TreeSystem.NodeArray[i], atoi(input));
            TreeSystem.NodeArray[i]->AdjacentNodes[a] = TreeSystem.NodeArray[atoi(input)];
            a++;
        }
    }
}

void TreeFree(void)
{
    TreeNode *ptr;
    int i = 0;
    
    if (TreeSystem.NodeArray == NULL)
        return;
    
    ptr = TreeSystem.NodeArray[0];
    while (ptr != NULL && i < TreeSystem.NodeCount)
    {
        if (ptr->AdjacentNodes != NULL)
            free(ptr->AdjacentNodes);
        if (ptr != NULL)
            free(ptr);
        
        ptr = TreeSystem.NodeArray[i + 1];
        i++;
    }
    free(TreeSystem.NodeArray);
    
    TreeSystem.NodeArray = NULL;
}

void AddToQuene(TreeNode *NewQueneNode)
{
    TreeNode *ptr = TreeSystem.QueneHead;
    
    if (NewQueneNode == NULL)
        return;
    
    while (ptr->Next != NULL)
    {
        ptr = ptr->Next;
    }
    ptr->Next = NewQueneNode;
}

void TreeBFS(void)
{
    int i;
    TreeSystem.QueneHead = TreeSystem.NodeArray[0];
    TreeSystem.QueneHead->VisitedFlag = 1;
    
    while (TreeSystem.QueneHead != NULL)
    {
        for (i = 0; i < TreeSystem.QueneHead->AdjCount; i++)
        {
            if (TreeSystem.QueneHead->AdjacentNodes[i]->VisitedFlag == 0)
                AddToQuene(TreeSystem.QueneHead->AdjacentNodes[i]);
            TreeSystem.QueneHead->AdjacentNodes[i]->VisitedFlag = 1;
        }
        printf("%i ", TreeSystem.QueneHead->NodeSign);
        TreeSystem.QueneHead = TreeSystem.QueneHead->Next;
    }
}

void StressTestRand(void)
{
    int i, a;
    FILE *F;
    TreeSystem.NodeCount = 5000;
    F = fopen("stresstest.txt", "w+");
    
    TreeInit(5000);
    for (i = 0; i < 5000; i++)
    {
        fprintf(F, "%i ", i);
        for (a = 0; a < rand() % 20; a++)
            fprintf(F, "%i ", 1 + rand() % 4999);
        fprintf(F, "\n");
    }
    
    fclose(F);
}

void TreeTestInit(char *addr)
{
    int i, NodeNum;
    char num[64];
    FILE *F;
    
    F = fopen(addr, "r");
    if (F == NULL)
        return;
    
    fgets(num, 64, F);
    NodeNum = atoi(num);
    
    if (NodeNum == 0)
    {
        TreeSystem.NodeArray = NULL;
        return;
    }
    
    TreeSystem.NodeArray = (TreeNode**)malloc(NodeNum * sizeof(TreeNode*));
    TreeSystem.NodeArray[0] = InitNode(0);
    TreeSystem.NodeCount = NodeNum;
    
    for (i = 1; i < NodeNum; i++)
    {
        TreeSystem.NodeArray[i] = InitNode(i);
    }
}

int main(int argc, const char * argv[]) {
    char NodeNumberstr[256];
    int i;
    struct timespec ts;
    unsigned long timestart, timeend;
    
    current_utc_time(&ts);
    timestart = ts.tv_sec;
    /*
    gets(NodeNumberstr);
    TreeSystem.NodeCount = atoi(NodeNumberstr);
    
    TreeInit(TreeSystem.NodeCount);
    TreeInput();
    TreeBFS();
    TreeFree();
     */
    /*
    StressTestRand();
    TreeInputForTest("stresstest.txt");
    for (i = 0; i < 5000; i++)
    {
        if (TreeSystem.NodeArray[i]->AdjCount > 0)
            AdjSort(TreeSystem.NodeArray[i]->AdjacentNodes, 0, TreeSystem.NodeArray[i]->AdjCount - 1);
    }
    TreeBFS();
    TreeFree();
    current_utc_time(&ts);
    timeend = ts.tv_sec;
    printf("\n %lu seconds spent on completion\n", timeend - timestart);
     */
    
    TreeTestInit("test3.txt");
    TreeInputForTest("test3.txt");
    TreeBFS();
    TreeFree();
    current_utc_time(&ts);
    timeend = ts.tv_sec;
    printf("\n %lu seconds spent on completion\n", timeend - timestart);
    
    return 0;
}
