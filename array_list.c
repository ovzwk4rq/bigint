#include <stdlib.h>
#include <array_list.h>

void	array_list_init(
		struct s_array_list *arr_list,
		void *(*alloc_node)(void))
{
	dllist_init(&arr_list->dllist, free);
	arr_list->alloc_node = alloc_node;
	arr_list->size = 0;
}

static int	add_one_node(
		struct s_dllist *list,
		void *(*alloc_node)(void))
{
	struct s_dllist_node	*new;

	new = alloc_node();
	if (!new)
		return (0);
	dllist_link_right(list, new);
	return (1);
}

static int	add_nodes(
		struct s_dllist *list,
		size_t n,
		void *(*alloc_node)(void))
{
	struct s_dllist	temp_list;

	dllist_init(&temp_list, free);
	while (n-- > 0)
		if (!add_one_node(&temp_list, alloc_node))
		{
			dllist_delete_all(&temp_list);
			return (0);
		}
	dllist_cat_right(list, &temp_list);
	return (1);
}

int	array_list_grow(struct s_array_list *arr_list, size_t change)
{
	size_t	new_nodes;

	new_nodes
		= (AL_CHUNK - 1 + arr_list->size + change) / AL_CHUNK
		- (AL_CHUNK - 1 + arr_list->size) / AL_CHUNK;
	if (!add_nodes(&arr_list->dllist, new_nodes, arr_list->alloc_node))
		return (0);
	arr_list->size += change;
	return (1);
}

int array_list_shrink(struct s_array_list *arr_list, size_t change)
{
	size_t	nodes_to_delete;

	nodes_to_delete
		= (AL_CHUNK - 1 + arr_list->size) / AL_CHUNK
		- (AL_CHUNK - 1 + arr_list->size - change) / AL_CHUNK;
	if (!dllist_delete_right(&arr_list->dllist, nodes_to_delete))
		return (0);
	arr_list->size -= change;
	return (1);
}

static void	index_to_cursor(
		struct s_array_list_cursor *cursor,
		struct s_array_list *arr_list,
		size_t index)
{
	cursor->arr_list = arr_list;
	cursor->node = dllist_get_nth_from_left(
			&arr_list->dllist,
			index / AL_CHUNK);
	cursor->index_in_chunk = index % AL_CHUNK;
	cursor->index = index;
}

void	array_list_index_to_cursor(
		struct s_array_list_cursor *cursor,
		struct s_array_list *arr_list,
		size_t index)
{
	if (index >= arr_list->size)
		array_list_grow(arr_list, index + 1 - arr_list->size);
	index_to_cursor(cursor, arr_list, index);
}

static void	move_forward(
		struct s_array_list_cursor *cursor,
		size_t dist)
{
	size_t	node_dist;

	node_dist = (cursor->index_in_chunk + dist) / AL_CHUNK;
	cursor->node = dllist_get_right(
			&cursor->arr_list->dllist,
			cursor->node,
			node_dist);
	cursor->index += dist;
	cursor->index_in_chunk = cursor->index % AL_CHUNK;
}

static void	move_backward(
		struct s_array_list_cursor *cursor,
		size_t dist)
{
	size_t	node_dist;

	node_dist = (AL_CHUNK - 1 - cursor->index_in_chunk + dist) / AL_CHUNK;
	cursor->node = dllist_get_left(
			&cursor->arr_list->dllist,
			cursor->node,
			node_dist);
	cursor->index -= dist;
	cursor->index_in_chunk = cursor->index % AL_CHUNK;
}

void	array_list_move_forward(
		struct s_array_list_cursor *cursor,
		size_t dist)
{
	size_t	next_index;

	next_index = cursor->index + dist;
	if (next_index + 1 > cursor->arr_list->size)
		array_list_grow(
				cursor->arr_list,
				next_index + 1 - cursor->arr_list->size);
	move_forward(cursor, dist);
}

int	array_list_move_backward(
		struct s_array_list_cursor *cursor,
		size_t dist)
{
	if (cursor->index < dist)
		return (0);
	move_backward(cursor, dist);
	return (1);
}
