#include "list_and_stack.h"
#include <stddef.h>
#include <stdlib.h>

int add_node_last(struct head *head, int data) {
	struct node *node = malloc(sizeof(struct node));
	if (!node) {
		return -1;
	}

	if (head->count_node == 0) {
		head->first = node;
		head->last = node;
	} else {
		head->last->next = node;
		head->last = node;
	}

	head->count_node += 1;
	node->next = NULL;
	node->data = data;

	return 0;
}

int add_node_first(struct head *head, int data) {
	struct node *node = malloc(sizeof(struct node));
	if (!node) {
		return -1;
	}

	node->next = head->first;

	if (head->count_node == 0) {
		head->first = node;
		head->last = node;
	} else {
		head->first = node;
	}

	head->count_node += 1;
	node->data = data;

	return 0;
}

void free_list(struct head *head) {
	int count_node = head->count_node;
	struct node *node = head->first;
	struct node *next_node = NULL;

	free(head);

	for (int i = 0; i < count_node; i++) {
		next_node = node->next;
		free(node);
		node = next_node;
	}
}

struct top_of_stack *push(struct top_of_stack *last_top_of_stack, struct head *list) {
	struct top_of_stack *new_top_of_stack = malloc(sizeof(struct top_of_stack));
	if (list == (void *)-1 || !new_top_of_stack) {
		return (void *)-1;
	}

	new_top_of_stack->prev = last_top_of_stack;
	new_top_of_stack->list = list;

	return new_top_of_stack;
}

struct head *pop(struct top_of_stack *last_top_of_stack) {
	if (!last_top_of_stack) {
		return NULL;
	}

	struct head *number = last_top_of_stack->list;

	return number;
}

void free_stack(struct top_of_stack *top_of_stack, int count_numbers_in_stack, char *symvol) {
	struct top_of_stack *new_top_of_stack = NULL;

	for (int i = 0; i < count_numbers_in_stack; i++) {
		new_top_of_stack = top_of_stack->prev;
		free_list(top_of_stack->list);
		free(top_of_stack);
		top_of_stack = new_top_of_stack;
	}

	free(symvol);
}
