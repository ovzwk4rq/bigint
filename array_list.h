#ifndef ARRAY_LIST_H
# define ARRAY_LIST_H
# include <stddef.h>
# include <dllist.h>
# define AL_CHUNK 64

struct s_array_list
{
	struct s_dllist	dllist;
	void			*(*alloc_node)(void);
	size_t			size;
};

struct s_array_list_cursor
{
	struct s_array_list		*arr_list;
	struct s_dllist_node	*node;
	size_t					index_in_chunk;
	size_t					index;
};

void	array_list_init(
		struct s_array_list *arr_list,
		void *(*alloc_node)(void));
int	array_list_grow(struct s_array_list *arr_list, size_t change);
int	array_list_shrink(struct s_array_list *arr_list, size_t change);
void	array_list_index_to_cursor(
		struct s_array_list_cursor *cursor,
		struct s_array_list *arr_list,
		size_t index);
void	array_list_move_forward(
		struct s_array_list_cursor *cursor,
		size_t dist);
int	array_list_move_backward(
		struct s_array_list_cursor *cursor,
		size_t dist);
#endif
