#include "linked_list.h"

struct List ListInit(void *data, size_t data_type_size) {
	struct List l = {0};
	l.head = malloc(sizeof *l.head);
	
	l.head->data = malloc(data_type_size);
	memcpy(l.head->data, data, data_type_size);
	l.head->next = NULL;

	l.data_type_size = data_type_size;
	l.num_nodes = 1;

	return l;
}

void ListAppend(struct List *list, void *data) {
	struct Node *itr = list->head;
	while(itr->next) {
		itr = itr->next;
	}

	itr->next = malloc(sizeof *itr);
	itr->next->data = malloc(list->data_type_size);
	memcpy(itr->next->data, data, list->data_type_size);
	itr->next->next = NULL;

	list->num_nodes++;
}

void ListDestroy(struct List *list) {
	struct Node *itr = list->head;
	for(int i = 0; i < list->num_nodes; i++) {
		struct Node *tmp_node = itr->next;
		free(itr->data);
		free(itr);
		itr = tmp_node;
	}
}
