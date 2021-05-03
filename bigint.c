#include <stdlib.h>
#include <array_list.h>
#include <bigint.h>
#define MSB 1 << 64

static void	*alloc_u64_array_node(void)
{
	return (malloc(sizeof(struct s_u64_array_node)));
}

static void	u64_array_list_set(struct s_array_list_cursor *cursor, uint64_t data)
{
	struct s_u64_array_node	*node;

	node = (struct s_u64_array_node *)cursor->node;
	node->array[cursor->index_in_chunk] = data;
}

static uint64_t	u64_array_list_get(struct s_array_list_cursor *cursor)
{
	struct s_u64_array_node *node;

	node = (struct s_u64_array_node *)cursor->node;
	return (node->array[cursor->index_in_chunk]);
}

static uint64_t	*u64al_element(struct s_array_list_cursor *cursor)
{
	struct s_u64_array_node	*node;

	node = (struct s_u64_array_node *)cursor->node;
	return (node->array + cursor->index_in_chunk);
}

static inline int	sign_of(int64_t n)
{
	return (n < 0);
}

void	bigint_set(struct s_bigint *n, int64_t value)
{
	struct s_array_list_cursor	cursor;

	array_list_index_to_cursor(&cursor, n->abs, 0);
	if (value < 0)
		*u64al_element(&cursor) = -(uint64_t)value;
	else
		*u64al_element(&cursor) = (uint64_t)value;
	if (*u64al_element(&cursor) & MSB)
	{
		*u64al_element(&cursor) &= ~MSB;
		array_list_move_forward(&cursor, 1);
		*u64al_element(&cursor) = 1;
	}
	n->sign = sign_of(value);
}

static void	index_to_many_cursor(
		struct s_array_list_cursor cursor[],
		struct s_array_list *arr_list[],
		size_t how_many,
		size_t index)
{
}

static void	move_many_cursor_forward(
		struct s_array_list_cursor cursor[],
		size_t how_many,
		size_t dist)
{
}

void	bigint_add(
		struct s_bigint *answer,
		struct s_bigint *n1,
		struct s_bigint *n2)
{
	struct s_array_list_cursor	cursor[3];
	struct s_array_list_cursor	*cursor_of_longer;
	struct s_array_list			*const arr_list[3] = {
		answer->abs, n1->abs, n2->abs};
	uint64_t					carry;

	if (n1->abs.size > n2->abs.size)
		cursor_of_longer = &cursor[N1];
	else
		cursor_of_longer = &cursor[N2];
	index_to_many_cursor(cursor, arr_list, 3, 0);
	carry = 0;
	while (*u64al_element(&cursor[N1])
			&& *u64al_element(&cursor[N2]))
	{
		*u64al_element(&cursor[ANSWER])
			= *u64al_element(&cursor[N1])
			+ *u64al_element(&cursor[N2])
			+ carry;
		carry = get_carry(...);
		move_many_cursor_forward(cursor, 3, 1);
	}
	while (u64al_element(cursor_of_longer))
	{
		*u64al_element(&cursor[ANSWER])
			= *u64al_element(cursor_of_longer)
			+ carry;
	}
	handle_last_carry(...);
}
