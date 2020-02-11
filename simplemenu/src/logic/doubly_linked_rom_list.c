/* Doubly Linked List implementation */

#include <stdio.h>
#include <stdlib.h>
#include "../headers/globals.h"
#include "../headers/definitions.h"

//Creates a new Node and returns pointer to it.
struct Node* GetNewNode(struct Rom rom) {
	struct Node* newNode= (struct Node*)malloc(sizeof(struct Node));
	newNode->data = rom;
	newNode->prev = NULL;
	newNode->next = NULL;
	return newNode;
}

//Inserts a Node at head of doubly linked list
//void InsertAtHead(struct Rom* rom) {
//	struct Node* newNode = GetNewNode(rom);
//	if(CURRENT_SECTION.head == NULL) {
//		CURRENT_SECTION.head = newNode;
//		return;
//	}
//	CURRENT_SECTION.head->prev = newNode;
//	newNode->next = CURRENT_SECTION.head;
//	CURRENT_SECTION.head = newNode;
//}

//Inserts a Node at tail of Doubly linked list
void InsertAtTail(struct Rom rom) {
	struct Node* temp = CURRENT_SECTION.head;
	struct Node* newNode = GetNewNode(rom);
	if(CURRENT_SECTION.head == NULL) {
		CURRENT_SECTION.head = newNode;
		return;
	}
	while(temp->next != NULL) {
		temp = temp->next; // Go To last Node
	}
	temp->next = newNode;
	newNode->prev = temp;
}

//Prints all the elements in linked list in forward traversal order
void PrintDoublyLinkedRomList() {
	struct Node* temp = CURRENT_SECTION.head;
	while(temp != NULL) {
		printf("%s \n",temp->data.name);
		temp = temp->next;
	}
	printf("\n");
}

// Takes head pointer of
// the linked list and index
// as arguments and return
// data at index
struct Rom* GetNthElement(int index)
{

	struct Node* current = CURRENT_SECTION.head;

    // the index of the
    // node we're currently
    // looking at
    int count = 0;
    while (current != NULL) {
        if (count == index) {
            return(&(current->data));
        }
        count++;
        current = current->next;
    }
    return NULL;
//    /* if we get to this line,
//    the caller was asking
//    for a non-existent element
//    so we assert fail */
//    assert(0);
}

struct Rom* getCurrentRom() {
	 return GetNthElement(CURRENT_GAME_NUMBER);
}

struct Node* GetNthNode(int index) {
	struct Node* current = CURRENT_SECTION.head;
    int count = 0;
    while (current != NULL) {
        if (count == index) {
            return(current);
        }
        count++;
        current = current->next;
    }
    return NULL;
}

struct Node *getCurrentNode() {
	return GetNthNode(CURRENT_GAME_NUMBER);
}

