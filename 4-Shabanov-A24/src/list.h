//
//  list.h
//  SPBPU-24-ALG-COURSE2_CPP
//
//  Created by Никита Шабанов on 11.10.2021.
//

/*
#ifndef list_h
#define list_h

typedef struct ListNode ListNode;
struct ListNode
{
    char Name[18]; // Searching for longer names is an exercise in futility.
    char Surname[49]; // Longest recorded surname in the world consists of 48 characters.
    char Date[11];
    int Workhours;
};

typedef struct LIST LIST;
struct LIST {
    struct ListNode data;
    struct LIST *Next;
};
 
int ListErasure(struct LIST *headelement, int elementcount);

ListNode NodeInit(char *date, char *surname, char *name, int workhours);

int DisplayList(LIST *headelement, int SumNumbHours);

LIST* HeadElementCreation(void);

int PriorityComparison(ListNode newelement, ListNode oldelement);

int NodeAddition(struct LIST **headelement, ListNode newdata);

int FileRead(LIST *headelement, char *filename);

#endif
*/

/* without this linker errors will occur. */

#ifdef __cplusplus
extern "C"
{
    #endif

    typedef struct ListNode ListNode;
    struct ListNode
    {
        char Name[18]; // Searching for longer names is an exercise in futility.
        char Surname[49]; // Longest recorded surname in the world consists of 48 characters.
        char Date[11];
        int Workhours;
    };

    typedef struct LIST LIST;
    struct LIST {
        struct ListNode data;
        struct LIST *Next;
    };
 
    int ListErasure(struct LIST *headelement, int elementcount);

    ListNode NodeInit(char *date, char *surname, char *name, int workhours);

    int DisplayList(LIST *headelement, int SumNumbHours);

    LIST* HeadElementCreation(void);

    int PriorityComparison(ListNode newelement, ListNode oldelement);

    int NodeAddition(struct LIST **headelement, ListNode newdata);

    int FileRead(LIST *headelement, char *filename);

    #ifdef __cplusplus
}
#endif
