#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdlib.h>
#include <string.h>

struct node {
	void *data;
	struct node *next;
};

struct list {
	struct node *head;
	size_t data_type_size;
	int num_nodes;
};

/* void *data: void pointer to inital data
 * size_t data_size: size int bytes of *data 
 */
struct list list_init(void *data, size_t data_size);
void list_append(struct list *list, void *data);
void list_destroy(struct list *list);

#endif
