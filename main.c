#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <dllist.h>
#include <array_list.h>

struct string_node
{
	struct s_dllist_node	node;
	char					*string;
};

int	add_right(struct s_dllist *list, char *string)
{
	struct string_node	*new;

	new = malloc(sizeof(struct string_node));
	if (!new)
		return (0);
	new->string = string;
	dllist_link_right(list, (struct s_dllist_node *)new);
	return (1);
}

void	dllist_test(void)
{
	struct s_dllist			list1;
	struct s_dllist			list2;
	struct s_dllist_node	*curr;

	dllist_init(&list1, free);
	add_right(&list1, "please");
	add_right(&list1, "work");
	add_right(&list1, "well");
	dllist_init(&list2, free);
	add_right(&list2, "this");
	add_right(&list2, "is");
	add_right(&list2, "hell");
	dllist_cat_right(&list1, &list2);
	printf("%lu\n", list1.size);
	curr = dllist_get_left_end(&list1);
	while (curr)
	{
		printf("%s ", ((struct string_node *)curr)->string);
		curr = dllist_get_right(&list1, curr, 4);
	}
	printf("\n");
	dllist_delete_all(&list1);
	printf("%lu\n", list1.size);
}

struct u64_array_node
{
	struct s_dllist_node	node;
	uint64_t				array[AL_CHUNK];
};

void	*alloc_u64_array_node(void)
{
	return (malloc(sizeof(struct u64_array_node)));
}

void	u64al_set(struct s_array_list_cursor *cursor, uint64_t n)
{
	struct u64_array_node	*node;

	node = (struct u64_array_node *)cursor->node;
	node->array[cursor->index_in_chunk] = n;
}

uint64_t	u64al_get(struct s_array_list_cursor *cursor)
{
	struct u64_array_node	*node;

	node = (struct u64_array_node *)cursor->node;
	return (node->array[cursor->index_in_chunk]);
}

void	array_list_test(void)
{
	struct s_array_list			arr_list;
	struct s_array_list_cursor	cursor;

	array_list_init(&arr_list, alloc_u64_array_node);
	array_list_index_to_cursor(&cursor, &arr_list, 0);
	for (int i = 0; i < 2300; i++)
	{
		u64al_set(&cursor, (uint64_t)i);
		array_list_move_forward(&cursor, 1);
	}
	array_list_index_to_cursor(&cursor, &arr_list, 0);
	for (int i = 0; i < 2300; i++)
	{
		printf("%d: %lu\n", i, u64al_get(&cursor));
		array_list_move_forward(&cursor, 1);
	}
	array_list_shrink(&arr_list, arr_list.size);
}

int	main(void)
{
	array_list_test();
	return (0);
}
