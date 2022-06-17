#include "linkedList.h"

struct list list_init(void *data, size_t data_type_size) {
	struct list l = {0};
	l.head = malloc(sizeof *l.head);
	
	l.head->data = malloc(data_type_size);
	memcpy(l.head->data, data, data_type_size);
	l.head->next = NULL;

	l.data_type_size = data_type_size;
	l.num_nodes = 1;

	return l;
}

void list_append(struct list *list, void *data) {
	struct node *itr = list->head;
	while(itr->next) {
		itr = itr->next;
	}

	itr->next = malloc(sizeof *itr);
	itr->next->data = malloc(list->data_type_size);
	memcpy(itr->next->data, data, list->data_type_size);
	itr->next->next = NULL;

	list->num_nodes++;
}

void list_destroy(struct list *list) {
	struct node *itr = list->head;
	for(int i = 0; i < list->num_nodes; i++) {
		struct node *tmp_node = itr->next;
		free(itr->data);
		free(itr);
		itr = tmp_node;
	}
}
