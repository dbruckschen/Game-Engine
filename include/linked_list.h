#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdlib.h>
#include <string.h>

struct Node {
	void *data;
	struct Node *next;
};

struct List {
	struct Node *head;
	size_t data_type_size;
	int num_nodes;
};

/* void *data: void pointer to inital data
 * size_t data_size: size int bytes of *data 
 */
__declspec(dllexport) struct List ListInit(void *data, size_t data_size);
__declspec(dllexport) void ListAppend(struct List *list, void *data);
__declspec(dllexport) void ListDestroy(struct List *list);

#endif
