#pragma once

struct head {
	int count_node;
	int number_sign;
	struct node *first;
	struct node *last;
};

struct node {
	int data;
	struct node *next;
};

struct top_of_stack {
	struct head *list;
	struct top_of_stack *prev;
};

int add_node_last(struct head *head, int data);

int add_node_first(struct head *head, int data);

void free_list(struct head *head);

struct top_of_stack *push(struct top_of_stack *last_top_of_stack, struct head *list);

struct head *pop(struct top_of_stack *last_top_of_stack);

void free_stack(struct top_of_stack *top_of_stack, int count_numbers_in_stack, char *symvol);
