#include "list_and_stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define POSITIVE 0

#define NEGATIVE 1

#define symvol_into_number(symvol) ((symvol) & 0x0F)

#define number_into_symvol ((char)(new_node->data | 0x30))

struct head *make_list_for_number(char number, char *symvol) {
	struct head *head = malloc(sizeof(struct head));
	if (!head) {
		return (void *)-1;
	}

	head->count_node = 0;
	head->first = NULL;
	head->last = NULL;

	if (number == '-') {
		head->number_sign = NEGATIVE;
	} else {
		head->number_sign = POSITIVE;
		if (!strchr("0123456789", number)) {
			free_list(head);
			return NULL;
		}
		if (add_node_first(head, symvol_into_number(number)) == -1) {
			free_list(head);
			return (void *)-1;
		};
	}

	while (symvol[0] != ' ' && symvol[0] != '\n') {
		if (!strchr("0123456789", symvol[0])) {
			free_list(head);
			return NULL;
		}
		if (add_node_first(head, symvol_into_number(symvol[0])) == -1) {
			free_list(head);
			return (void *)-1;
		}
		scanf("%c", &symvol[0]);
	}

	return head;
}

int comparison_of_numbers(struct head *number_1, struct head *number_2) {
	int len_lists = (number_1->count_node > number_2->count_node) ? number_1->count_node : number_2->count_node;
	if (len_lists == 0) {
		return 0;
	}

	int bigger_number = 1;
	struct node *node_1 = number_1->first;
	struct node *node_2 = number_2->first;
	int *list_1 = malloc(sizeof(int) * len_lists);
	int *list_2 = malloc(sizeof(int) * len_lists);
	if (!list_1 || !list_2) {
		return -1;
	}

	for (int i = 0; i < len_lists; i++) {
		if (!node_1) {
			list_1[i] = 0;
		} else {
			list_1[i] = node_1->data;
			node_1 = node_1->next;
		}
		if (!node_2) {
			list_2[i] = 0;
		} else {
			list_2[i] = node_2->data;
			node_2 = node_2->next;
		}
	}

	for (int i = len_lists - 1; i > -1; --i) {
		if (list_1[i] == list_2[i]) {
			continue;
		} else {
			bigger_number = (list_1[i] > list_2[i]) ? 1 : 2;
			break;
		}
	}

	free(list_1);
	free(list_2);
	return bigger_number;
}

struct head *sub(struct head *number_1, struct head *number_2) {
	if (!number_1->first | !number_2->first) {
		return NULL;
	}

	struct head *new_head = malloc(sizeof(struct head));
	if (!new_head) {
		return (void *)-1;
	}

	int len_new_list = (number_1->count_node > number_2->count_node) ? number_1->count_node : number_2->count_node;
	struct node *node_1 = number_1->first;
	struct node *node_2 = number_2->first;
	struct node *occupying_rank = NULL;
	new_head->first = NULL;
	new_head->last = NULL;
	new_head->count_node = 0;
	new_head->number_sign = POSITIVE;

	for (int i = 0; i < len_new_list; i++) {
		if (node_1->data >= node_2->data) {
			if (add_node_last(new_head, node_1->data - node_2->data) == -1) {
				free_list(new_head);
				return (void *)-1;
			};
		} else {
			node_1->data += 10;
			if (add_node_last(new_head, node_1->data - node_2->data) == -1) {
				free_list(new_head);
				return (void *)-1;
			};
			occupying_rank = node_1->next;
			while (occupying_rank->data == 0) {
				occupying_rank->data = 9;
				occupying_rank = occupying_rank->next;
			}
			occupying_rank->data -= 1;
		}

		if ((!node_1->next) && (!node_2->next)) {
			return new_head;
		}
		if (!node_1->next) {
			node_1->data = 0;
		} else {
			node_1 = node_1->next;
		}
		if (!node_2->next) {
			node_2->data = 0;
		} else {
			node_2 = node_2->next;
		}
	}

	return NULL;
}

struct head *symm(struct head *number_1, struct head *number_2) {
	if (!number_1->first | !number_2->first) {
		return NULL;
	}

	struct head *new_head = malloc(sizeof(struct head));
	if (!new_head) {
		return (void *)-1;
	}

	int len_new_list = (number_1->count_node > number_2->count_node) ? number_1->count_node : number_2->count_node;
	struct node *node_1 = number_1->first;
	struct node *node_2 = number_2->first;
	int overflow_last_node = 0;
	new_head->first = NULL;
	new_head->last = NULL;
	new_head->count_node = 0;
	new_head->number_sign = POSITIVE;

	for (int i = 0; i < len_new_list; i++) {
		if (add_node_last(new_head, (node_1->data + node_2->data + overflow_last_node) % 10) == -1) {
			free_list(new_head);
			return (void *)-1;
		};
		overflow_last_node = (node_1->data + node_2->data + overflow_last_node) / 10;
		if ((!node_1->next) && (!node_2->next)) {
			if (overflow_last_node != 0) {
				if (add_node_last(new_head, overflow_last_node) == -1) {
					free_list(new_head);
					return (void *)-1;
				};
			}
			return new_head;
		}

		if (!node_1->next) {
			node_1->data = 0;
		} else {
			node_1 = node_1->next;
		}
		if (!node_2->next) {
			node_2->data = 0;
		} else {
			node_2 = node_2->next;
		}
	}

	return NULL;
}

struct head *multiplication(struct head *number_1, struct head *number_2) {
	if (!number_1->first | !number_2->first) {
		return NULL;
	}

	struct head *new_head = NULL;
	struct top_of_stack *top_of_stack = NULL;
	struct top_of_stack *new_top_of_stack = NULL;
	struct top_of_stack *last_top_of_stack = NULL;
	struct node *node_1 = number_1->first;
	struct node *node_2 = number_2->first;
	int overflow_last_node = 0;
	struct head *number_22 = NULL;
	struct head *number_11 = NULL;
	struct head *return_list = NULL;

	for (int i = 0; i < number_1->count_node; i++) {
		new_head = malloc(sizeof(struct head));
		if (!new_head) {
			return (void *)-1;
		}

		overflow_last_node = 0;
		new_head->first = NULL;
		new_head->last = NULL;
		new_head->count_node = 0;
		new_head->number_sign = POSITIVE;

		for (int k = 0; k < i; k++) {
			if (add_node_last(new_head, 0) == -1) {
				free_list(new_head);
				free_stack(last_top_of_stack, i, NULL);
				return (void *)-1;
			};
		}

		for (int j = 0; j < number_2->count_node; j++) {
			if (add_node_last(new_head, (node_1->data * node_2->data + overflow_last_node) % 10) == -1) {
				free_list(new_head);
				free_stack(last_top_of_stack, i, NULL);
				return (void *)-1;
			};

			overflow_last_node = (node_1->data * node_2->data + overflow_last_node) / 10;
			node_2 = node_2->next;
		}

		if (overflow_last_node != 0) {
			if (add_node_last(new_head, overflow_last_node) == -1) {
				free_list(new_head);
				free_stack(last_top_of_stack, i, NULL);
				return (void *)-1;
			};
		}

		last_top_of_stack = top_of_stack;
		top_of_stack = push(top_of_stack, new_head);
		if (top_of_stack == (void *)-1) {
			free_list(new_head);
			free_stack(last_top_of_stack, i, NULL);
			return (void *)-1;
		}

		new_head = NULL;
		node_1 = node_1->next;
		node_2 = number_2->first;
	}

	for (int i = 0; i < number_1->count_node - 1; i++) {
		new_top_of_stack = top_of_stack->prev;
		number_22 = pop(top_of_stack);
		free(top_of_stack);
		top_of_stack = new_top_of_stack;

		new_top_of_stack = top_of_stack->prev;
		number_11 = pop(top_of_stack);
		free(top_of_stack);
		top_of_stack = new_top_of_stack;

		last_top_of_stack = top_of_stack;
		top_of_stack = push(top_of_stack, symm(number_22, number_11));
		if (top_of_stack == (void *)-1) {
			free_stack(last_top_of_stack, number_1->count_node - i - 2, NULL);
			free_list(number_22);
			free_list(number_11);
			return (void *)-1;
		}
		free_list(number_22);
		free_list(number_11);
	}

	return_list = top_of_stack->list;
	free(top_of_stack);

	return return_list;
}

#define clear                                                                                                          \
	{                                                                                                              \
		free_list(number_1);                                                                                   \
		free_list(number_2);                                                                                   \
		free_stack(last_top_of_stack, count_numbers_for_operation - 2, symvol);                                \
		return 5;                                                                                              \
	}

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "incorrect number of command line arguments\n");
		return 1;
	}

	if (strcmp(argv[1], "--infix") == 0) {
		fprintf(stderr, "the application does not support infix notation\n");
		return 2;
	}

	if (strcmp(argv[1], "--revpol") != 0) {
		fprintf(stderr, "invalid command line arguments\n");
		return 1;
	}

	struct top_of_stack *top_of_stack = NULL;
	struct top_of_stack *new_top_of_stack = NULL;
	struct top_of_stack *last_top_of_stack = NULL;
	struct head *number_2 = NULL;
	struct head *number_1 = NULL;
	int count_numbers_for_operation = 0;
	struct head *number_list = NULL;
	char number;
	int start_of_line = 0;
	struct head *result = NULL;
	struct node *new_node = NULL;
	char *new_number = NULL;

	char *symvol = malloc(sizeof(char));
	if (!symvol) {
		return 5;
	}

	do {
		scanf("%c", &number);
		scanf("%c", &symvol[0]);

		if ((strchr("+-*/", number)) && (symvol[0] == ' ' || symvol[0] == '\n')) {

			if (count_numbers_for_operation >= 2) {

				new_top_of_stack = top_of_stack->prev;
				number_2 = pop(top_of_stack);
				free(top_of_stack);
				top_of_stack = new_top_of_stack;

				new_top_of_stack = top_of_stack->prev;
				number_1 = pop(top_of_stack);
				free(top_of_stack);
				top_of_stack = new_top_of_stack;

				last_top_of_stack = top_of_stack;

				if (number == '+') {
					if (number_1->number_sign == POSITIVE && number_2->number_sign == POSITIVE) {

						top_of_stack = push(top_of_stack, symm(number_1, number_2));
						if (top_of_stack == (void *)-1)
							clear;

					} else if (number_1->number_sign == NEGATIVE &&
						   number_2->number_sign == POSITIVE) {

						if (comparison_of_numbers(number_1, number_2) == 1) {

							top_of_stack = push(top_of_stack, sub(number_1, number_2));
							if (top_of_stack == (void *)-1)
								clear;

							top_of_stack->list->number_sign = NEGATIVE;

						} else if (comparison_of_numbers(number_1, number_2) == 2) {

							top_of_stack = push(top_of_stack, sub(number_2, number_1));
							if (top_of_stack == (void *)-1)
								clear;

						} else {
							clear;
						}
					} else if (number_1->number_sign == POSITIVE &&
						   number_2->number_sign == NEGATIVE) {

						if (comparison_of_numbers(number_1, number_2) == 1) {

							top_of_stack = push(top_of_stack, sub(number_1, number_2));
							if (top_of_stack == (void *)-1)
								clear;

						} else if (comparison_of_numbers(number_1, number_2) == 2) {

							top_of_stack = push(top_of_stack, sub(number_2, number_1));
							if (top_of_stack == (void *)-1)
								clear;

							top_of_stack->list->number_sign = NEGATIVE;

						} else {
							clear;
						}
					} else {

						top_of_stack = push(top_of_stack, symm(number_1, number_2));
						if (top_of_stack == (void *)-1)
							clear;

						top_of_stack->list->number_sign = NEGATIVE;
					}
				} else if (number == '-') {
					if (number_1->number_sign == POSITIVE && number_2->number_sign == POSITIVE) {

						if (comparison_of_numbers(number_1, number_2) == 1) {

							top_of_stack = push(top_of_stack, sub(number_1, number_2));
							if (top_of_stack == (void *)-1)
								clear;

						} else if (comparison_of_numbers(number_1, number_2) == 2) {

							top_of_stack = push(top_of_stack, sub(number_2, number_1));
							if (top_of_stack == (void *)-1)
								clear;

							top_of_stack->list->number_sign = NEGATIVE;

						} else {
							clear;
						}
					} else if (number_1->number_sign == NEGATIVE &&
						   number_2->number_sign == POSITIVE) {

						top_of_stack = push(top_of_stack, symm(number_1, number_2));
						if (top_of_stack == (void *)-1)
							clear;

						top_of_stack->list->number_sign = NEGATIVE;

					} else if (number_1->number_sign == POSITIVE &&
						   number_2->number_sign == NEGATIVE) {

						top_of_stack = push(top_of_stack, symm(number_1, number_2));
						if (top_of_stack == (void *)-1)
							clear;

					} else {
						if (comparison_of_numbers(number_1, number_2) == 1) {

							top_of_stack = push(top_of_stack, sub(number_1, number_2));
							if (top_of_stack == (void *)-1)
								clear;

							top_of_stack->list->number_sign = NEGATIVE;

						} else if (comparison_of_numbers(number_1, number_2) == 2) {

							top_of_stack = push(top_of_stack, sub(number_2, number_1));
							if (top_of_stack == (void *)-1)
								clear;

						} else {
							clear;
						}
					}
				} else if (number == '*') {

					top_of_stack = push(top_of_stack, multiplication(number_1, number_2));
					if (top_of_stack == (void *)-1)
						clear;

					if ((number_1->number_sign == NEGATIVE && number_2->number_sign == POSITIVE) ||
					    (number_1->number_sign == POSITIVE && number_2->number_sign == NEGATIVE)) {

						top_of_stack->list->number_sign = 1;
					}
				} else {

					fprintf(stderr, "the application does not "
							"support the '-' operation\n");

					free_list(number_1);
					free_list(number_2);
					free_stack(top_of_stack, count_numbers_for_operation - 2, symvol);
					return 3;
				}

				count_numbers_for_operation -= 1;

				free_list(number_1);
				free_list(number_2);

			} else {

				fprintf(stderr, "Number is missed\n");

				free_stack(top_of_stack, count_numbers_for_operation, symvol);
				return 4;
			}
		} else {
			if (!(number_list = make_list_for_number(number, symvol))) {

				fprintf(stderr, "wrong number\n");

				free_stack(top_of_stack, count_numbers_for_operation, symvol);
				return 4;
			}
			if (number_list == (void *)-1) {
				free_stack(top_of_stack, count_numbers_for_operation, symvol);
				return 5;
			}

			last_top_of_stack = top_of_stack;
			top_of_stack = push(top_of_stack, number_list);

			if (top_of_stack == (void *)-1) {
				free_stack(last_top_of_stack, count_numbers_for_operation, symvol);
				return 5;
			}

			count_numbers_for_operation += 1;
		}
	} while (symvol[0] != '\n' && number != '\n');

	if (count_numbers_for_operation != 1) {
		fprintf(stderr, "incorrect expression entered\n");
		free_stack(top_of_stack, count_numbers_for_operation, symvol);
		return 4;
	}

	start_of_line = 0;
	result = pop(top_of_stack);
	new_node = result->first;

	new_number = malloc(sizeof(char) * result->count_node);
	if (!new_number) {
		free_stack(top_of_stack, count_numbers_for_operation, symvol);
		return 5;
	}

	for (int i = result->count_node - 1; i > -1; --i) {
		new_number[i] = number_into_symvol;
		new_node = new_node->next;
	}

	while (new_number[start_of_line] == '0') {
		if (start_of_line == result->count_node - 1) {
			break;
		}
		start_of_line++;
	}

	if (result->number_sign == NEGATIVE) {
		printf("-");
	}

	for (int i = 0; i < result->count_node - start_of_line; i++) {
		printf("%c", new_number[i + start_of_line]);
	}

	free(new_number);
	free_stack(top_of_stack, count_numbers_for_operation, symvol);
	return 0;
}
