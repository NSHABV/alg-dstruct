//
//  test.cpp
//  SPBPU-24-ALG-COURSE2
//
//  Created by Никита Шабанов on 11.10.2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtest/gtest.h>
#include "list.h"

char filename[15] = "namefile.txt";

/* The main function here is commented to allow the main function in list.c to be compiled without errors. */

/*
int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
*/

TEST(FileRead, HandlesInputOfNullList)
{
    EXPECT_EQ(FileRead(NULL, filename), 0);
}

TEST(FileRead, HandlesInputOfEmptyFileName)
{
    LIST *head;
    head = HeadElementCreation();
    EXPECT_EQ(FileRead(head, NULL), 0);
    free(head);
}

TEST(FileRead, HandlesInputOfNonexistentFilename)
{
    LIST *head;
    char a[10] = "aaaa";
    head = HeadElementCreation();
    EXPECT_EQ(FileRead(head, a), 0);
    free(head);
}

TEST(FileRead, HandlesInputOfUnformattedFile)
{
    LIST *head;
    char a[20] = "wrongformattest.txt";
    head = HeadElementCreation();
    EXPECT_EQ(FileRead(head, a), 1); // file reading is supposed to work as usual
    free(head->Next);
    free(head);
}

TEST(FileRead, HandlesInputOfEmptyFile)
{
    LIST *head;
    char a[20] = "emptyfiletest.txt";
    head = HeadElementCreation();
    EXPECT_EQ(FileRead(head, a), 1); // while loop doesnt begin, 1 is returned
    free(head);
}


TEST(NodeAddition, HandlesEmptyHead)
{
    ListNode newdata = NodeInit("Date", "Surname", "Name", -1);
    EXPECT_EQ(NodeAddition(NULL, newdata), 0);
}


TEST(NodeAddition, HandlesEmptyData) {
    LIST *head;
    ListNode a;
    head = HeadElementCreation();
    EXPECT_EQ(NodeAddition(&head, a), 1); // Empty elements can be added
    free(head->Next);
    free(head);
}

TEST(NodeAddition, AddingTwoExactElements)
{
    LIST *head;
    ListNode a = NodeInit("Date", "Surname", "Name", -1);
    ListNode b = NodeInit("Date", "Surname", "Name", -1);
    head = HeadElementCreation();
    EXPECT_EQ(NodeAddition(&head, a), 1);
    EXPECT_EQ(NodeAddition(&head, b), 1);
    free(head->Next->Next);
    free(head->Next);
    free(head);
}

TEST(PriorityComparison, HandlesTwoExactElements)
{
    ListNode element1 = NodeInit("Date", "Surname", "Name", 1);
    ListNode element2 = NodeInit("Date", "Surname", "Name", 1);
    EXPECT_EQ(PriorityComparison(element1, element2), 1);
}

TEST(PriorityComparison, ComparingDifferentWorkhours1)
{
    ListNode element1 = NodeInit("Date", "Surname", "Name", 11);
    ListNode element2 = NodeInit("Date", "Surname", "Name", 12);
    EXPECT_EQ(PriorityComparison(element1, element2), 0);
}

TEST(PriorityComparison, ComparingDifferentWorkhours2)
{
    ListNode element1 = NodeInit("Date", "Surname", "Name", 12);
    ListNode element2 = NodeInit("Date", "Surname", "Name", 11);
    EXPECT_EQ(PriorityComparison(element1, element2), 1);
}

TEST(PriorityComparison, ComparingSameSurnamesDifferentLength)
{
    ListNode element1 = NodeInit("Date", "Abcde", "Name", 11);
    ListNode element2 = NodeInit("Date", "Abcdef", "Name", 11);
    EXPECT_EQ(PriorityComparison(element1, element2), 1);
}

TEST(PriorityComparison, ComparingDifferentSurnames)
{
    ListNode element1 = NodeInit("Date", "Abcde", "Name", 11);
    ListNode element2 = NodeInit("Date", "Bcdef", "Name", 11);
    EXPECT_EQ(PriorityComparison(element1, element2), 1);
}

TEST(DisplayList, HandlesNullInput)
{
    EXPECT_EQ(DisplayList(NULL, 0), 0);
}

TEST(DisplayList, HandlesOutputOfOneElement)
{
    LIST *head;
    head = HeadElementCreation();
    EXPECT_EQ(DisplayList(head, 0), 1);
    free(head);
}

TEST(DisplayList, HandlesDenialOfUnsufficientWorkhours)
{
    LIST *head;
    head = HeadElementCreation();
    EXPECT_EQ(DisplayList(head, 10000), 0);
    free(head);
}

TEST(DisplayList, HandlesOutputOfEmptyElements)
{
    LIST *head;
    LIST *nelement = (LIST *)malloc(sizeof(LIST));
    ListNode newdata;
    head = HeadElementCreation();
    head->Next = nelement;
    nelement->data = newdata;
    nelement->Next = NULL;
    EXPECT_EQ(DisplayList(head, 0), 1);
    free(nelement);
    free(head);
}

TEST(ListErasure, HandlesNullListErasure)
{
    EXPECT_EQ(ListErasure(NULL, 0), 0);
}

TEST(ListErasure, HandlesNullErasureWithCounter)
{
    EXPECT_EQ(ListErasure(NULL, 1), 1);
}

TEST(ListErasure, HandlesOneElementDeletion)
{
    LIST *head;
    head = HeadElementCreation();
    EXPECT_EQ(ListErasure(head, 0), 1);
}
