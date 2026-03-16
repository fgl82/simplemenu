#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/definitions.h"
#include "../headers/globals.h"
#include "../headers/logic.h"

struct Node* GetNewNode(void *data, NodeType nodeType) {
	struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
	newNode->data = data;
	newNode->nodeType = nodeType;
	newNode->prev = NULL;
	newNode->next = NULL;
	return newNode;
}

void InsertAtTail(struct Rom *rom) {
	struct Node* temp = CURRENT_SECTION.head;
	struct Node* newNode = GetNewNode(rom, NODE_TYPE_ROM);
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


void cleanListForSection(struct MenuSection *section) {
	struct Node *current = section->head;
	while (current != NULL) {
		struct Node *next = current->next;
		if (current->data != NULL) {
			struct Rom *rom = (struct Rom *)current->data;
			if (rom->alias != NULL && strlen(rom->alias) > 0) {
				// We need to be careful here because loadGameList might set alias to a literal "\0"
				// but it also mallocs it in many places. 
				// However, in loadGameList:
				// if (strcmp(ptr,"(null)")==0) { free(rom->alias); rom->alias = "\0"; }
				// This is dangerous. Let's assume for now that if it's not the literal "\0" it's heap.
				// Actually, looking at loadGameList, it always mallocs rom->alias first.
				if (rom->alias[0] != '\0') {
					free(rom->alias);
				}
			}
			if (rom->name != NULL) {
				free(rom->name);
			}
			if (rom->directory != NULL) {
				free(rom->directory);
			}
			free(rom);
		}
		free(current);
		current = next;
	}
	section->head = NULL;
	section->tail = NULL;
	section->currentGameNode = NULL;
}

void InsertAtTailInSection(struct MenuSection *section, struct Rom *rom) {
	struct Node* temp = section->head;
	struct Node* newNode = GetNewNode(rom, NODE_TYPE_ROM);
	if(section->head == NULL) {
		section->head = newNode;
		return;
	}
	while(temp->next != NULL) {
		temp = temp->next; // Go To last Node
	}
	temp->next = newNode;
	newNode->prev = temp;
}

struct Rom* GetNthElement(int index)
{
	struct Node* current = CURRENT_SECTION.head;
	int count = 0;
	while (current != NULL) {
		if (count == index) {
			return (struct Rom *)current->data;
		}
		count++;
		current = current->next;
	}
	return NULL;
}

struct Rom* getCurrentRom() {
	return GetNthElement(CURRENT_GAME_NUMBER);
}

void InsertFavoriteAtTail(struct Favorite *favorite) {
	struct Node* temp = favoritesHead;
	struct Node* newNode = GetNewNode(favorite, NODE_TYPE_FAVORITE);
	if(favoritesHead == NULL) {
		favoritesHead = newNode;
		favoritesTail = newNode;
		return;
	}
	while(temp->next != NULL) {
		temp = temp->next; // Go To last Node
	}
	temp->next = newNode;
	newNode->prev = temp;
	favoritesTail = newNode;
}

void RemoveFavoriteAt(int index) {
	struct Node* current = favoritesHead;
	int count = 0;
	
	// Find the node at the specified index
	while (current != NULL && count < index) {
		current = current->next;
		count++;
	}
	
	// If we found the node at the index
	if (current != NULL) {
		// Remove the node from the list
		if (current->prev != NULL) {
			current->prev->next = current->next;
		} else {
			// This is the head node
			favoritesHead = current->next;
		}
		
		if (current->next != NULL) {
			current->next->prev = current->prev;
		} else {
			// This is the tail node
			favoritesTail = current->prev;
		}
		
		// Free the favorite data and node
		struct Favorite* favorite = (struct Favorite*)current->data;
		if (favorite != NULL) {
			free(favorite);
		}
		free(current);
	}
}

struct Favorite* GetNthFavorite(int index) {
	struct Node* current = favoritesHead;
	int count = 0;
	while (current != NULL) {
		if (count == index) {
			return (struct Favorite*)current->data;
		}
		count++;
		current = current->next;
	}
	return NULL;
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

char *getCurrentSectionExistingLetters() {
	char *letters=calloc(29,1);
	struct Node* temp = CURRENT_SECTION.head;
	int hadNumbers = 0;
	while(temp != NULL) {
		char *name = getFileNameOrAlias(temp->data);
		char *upperInitialLetter = malloc(2);
		upperInitialLetter[0]=toupper(name[0]);
		upperInitialLetter[1]='\0';
		if (strstr(letters,upperInitialLetter)==NULL) {
			if(isdigit(upperInitialLetter[0])||!isalpha(upperInitialLetter[0])) {
				if(!hadNumbers) {
					strcat(letters,"#");
				}
				hadNumbers=1;
			} else {
				strcat(letters,upperInitialLetter);
			}
		}
		free(name);
		free(upperInitialLetter);
		temp = temp->next;
	}
	return letters;
}

void cleanFavoritesList() {
	struct Node *current = favoritesHead;
	while (current != NULL) {
		struct Node *next = current->next;
		if (current->data != NULL) {
			// No need to free individual strings in Favorite since they're fixed arrays
			// Just free the Favorite struct itself
			free(current->data);
		}
		free(current);
		current = next;
	}
	favoritesHead = NULL;
	favoritesTail = NULL;
	favoritesSize = 0;
}
