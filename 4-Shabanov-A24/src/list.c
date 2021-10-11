//
//  list.c
//  SPBPU-24-ALG-COURSE2
//
//  Created by Никита Шабанов on 07.09.2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

int ListErasure(struct LIST *headelement, int elementcount)
{
    LIST *nextelement;
    if (headelement == NULL && elementcount == 0)
    {
        printf("May not delete non-existent list.\n");
        return 0;
    }
    else if (headelement == NULL && elementcount != 0)
    {
        printf("List Erasure complete. Erased %i elements.\n", elementcount);
        return 1;
    }
    nextelement = headelement->Next;
    free(headelement);
    ListErasure(nextelement, elementcount + 1);
    return 1;
}

/* No logic. Therefore, according to task specifications, Tests are not required. */
ListNode NodeInit(char *date, char *surname, char *name, int workhours)
{
    ListNode newnode;
    strcpy(newnode.Name, name);
    strcpy(newnode.Surname, surname);
    strcpy(newnode.Date, date);
    newnode.Workhours = workhours;
    return newnode;
}

int DisplayList(LIST *headelement, int SumNumbHours)
{
    LIST *temp;
    int Workhourcount = 0;
    int Summaryworkcount = 1; // First element has -1 value.
    if (headelement == NULL)
    {
        printf("May not display empty list.\n");
        return 0;
    }
    temp = headelement;
    while (temp != NULL)
    {
        Workhourcount = temp->data.Workhours;
        Summaryworkcount += Workhourcount;
        temp = temp->Next;
    }
    if (Summaryworkcount < SumNumbHours)
    {
        printf("Summary work of workers is less than desired parameter value.\n");
        return 0;
    }
    temp = headelement;
    while (temp != NULL)
    {
        Workhourcount = temp->data.Workhours;
        Summaryworkcount += Workhourcount;
        if (Workhourcount > 0)
        {
            printf("%s %s %s %d\n", temp->data.Date, temp->data.Surname, temp->data.Name, Workhourcount);
        }
        temp = temp->Next;
    }
    return 1;
}

/* No logic. Therefore, according to task specifications, Tests are not required. */
LIST* HeadElementCreation(void)
{
    LIST *nelement = (LIST *)malloc(sizeof(LIST));
    ListNode newdata;
    if (nelement == NULL)
    {
        printf("Memory allocation failure.\n");
        return NULL;
    }
    newdata = NodeInit("Date", "Surname", "Name", -1);
    nelement->data = newdata;
    nelement->Next = NULL;
    return nelement;
}

/* I am assuming that Surnames have same registry. Both start with upper-case letters and continue with lower-case letters. */
int PriorityComparison(ListNode newelement, ListNode oldelement)
{
    int i = 0;
    if (newelement.Workhours < oldelement.Workhours)
    {
        return 0;
    }
    else if (newelement.Workhours > oldelement.Workhours)
    {
        return 1;
    }
    else if (newelement.Workhours == oldelement.Workhours)
    {
        while (newelement.Surname[i] == oldelement.Surname[i] && newelement.Surname[i] != '\0' && oldelement.Surname[i] != '\0')
            i++;
        if (newelement.Surname[i] > oldelement.Surname[i])
        {
            return 0;
        }
        else if (newelement.Surname[i] < oldelement.Surname[i])
        {
            return 1;
        }
        else if (newelement.Surname[i - 1] == oldelement.Surname[i - 1])
        {
            i = 0;
            while (newelement.Name[i] == oldelement.Name[i] && newelement.Name[i] != '\0' && oldelement.Name[i] != '\0')
                i++;
            if (newelement.Name[i] > oldelement.Name[i])
            {
                return 0;
            }
            else if (newelement.Name[i] < oldelement.Name[i])
            {
                return 1;
            }
            else if (newelement.Name[i - 1] == oldelement.Name[i - 1])
            {
                return 1;
            }
        }
    }
    return 1;
}

int NodeAddition(struct LIST **headelement, ListNode newdata)
{
    LIST *nelement = (LIST *)malloc(sizeof(LIST));
    LIST **temp = headelement;
    
    if (nelement == NULL)
    {
        printf("Memory allocation failure.\n");
        return 0;
    }
    
    if (headelement == NULL)
    {
        printf("May not add node to an non-existent list.\n");
        free(nelement);
        return 0;
    }
    
    if ((*headelement) == NULL)
    {
        printf("May not add node to an non-existent list.\n");
        free(nelement);
        return 0;
    }
    
    nelement->data = newdata;
    
    if ((*temp)->Next == NULL)
    {
        (*temp)->Next = nelement;
        nelement->Next = NULL;
        return 1;
    }
    
    while (PriorityComparison(newdata, (*temp)->Next->data) == 0)
    {
      if ((*temp)->Next->Next != NULL)
      {
          temp = &(*temp)->Next;
      }
      else if ((*temp)->Next->Next == NULL && (*temp)->Next != NULL)
      {
          temp = &(*temp)->Next;
          (*temp)->Next = nelement;
          nelement->Next = NULL;
          return 1;
      }
    }
  
    if (PriorityComparison(newdata, (*temp)->Next->data) == 1)
    {
        nelement->Next = (*temp)->Next;
        (*temp)->Next = nelement;
        return 1;
    }
    return 0;
}

int FileRead(LIST *headelement, char *filename)
{
    FILE *readfile;
    char tempName[18];
    char tempSurname[49];
    char tempDate[11];
    int tempWorkhours;
    ListNode temp;
    
    if (filename == NULL)
    {
        printf("Impossible to open a file with no name provided.\n");
        return 0;
    }
    readfile = fopen(filename, "r");
    if (readfile == NULL)
    {
        printf("File read failure.\n");
        return 0;
    }
    if (headelement == NULL)
    {
        fclose(readfile);
        printf("Can't read file when there's no list to add elements to.\n");
        return 0;
    }
    
    while (fscanf(readfile, "%s%s%s%d", tempDate, tempSurname, tempName, &tempWorkhours) != EOF)
    {
        temp = NodeInit(tempDate, tempSurname, tempName, tempWorkhours);
        NodeAddition(&headelement, temp);
    }
    return 1;
}

int main(int argc, char** argv)
{
    LIST *head;
    int NHours = 0;
    char filename[15] = "namefile.txt";
  
    printf("What is desired amount of hours?\n");
    scanf("%i", &NHours);
    
    head = HeadElementCreation();
    if (head == NULL)
    {
        return 0;
    }
    if (FileRead(head, filename) == 0)
    {
        ListErasure(head, 0);
    }
  
    DisplayList(head, NHours);
    ListErasure(head, 0);
}
