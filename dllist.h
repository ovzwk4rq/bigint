#ifndef DLLIST_H
# define DLLIST_H
# include <stddef.h>

struct s_dllist_node
{
	struct s_dllist_node	*left;
	struct s_dllist_node	*right;
};

struct s_dllist
{
	struct s_dllist_node	entry;
	size_t					size;
	void					(*free_node)(void *);
};

void	dllist_init(
		struct s_dllist *list,
		void (*free_node)(void *));
void	dllist_link_left(struct s_dllist *list, struct s_dllist_node *new);
void	dllist_link_right(struct s_dllist *list, struct s_dllist_node *new);
int	dllist_delete_left(struct s_dllist *list, size_t n);
int	dllist_delete_right(struct s_dllist *list, size_t n);
void	dllist_delete_left_from(
		struct s_dllist *list,
		struct s_dllist_node *curr);
void	dllist_delete_right_from(
		struct s_dllist *list,
		struct s_dllist_node *curr);
void	dllist_delete_all(struct s_dllist *list);
struct s_dllist_node	*dllist_get_left_end(struct s_dllist *list);
struct s_dllist_node	*dllist_get_right_end(struct s_dllist *list);
struct s_dllist_node	*dllist_get_left(
		struct s_dllist *list,
		struct s_dllist_node *curr,
		size_t n);
struct s_dllist_node	*dllist_get_right(
		struct s_dllist *list,
		struct s_dllist_node *curr,
		size_t n);
struct s_dllist_node	*dllist_get_nth_from_left(
		struct s_dllist *list,
		size_t index);
struct s_dllist_node	*dllist_get_nth_from_right(
		struct s_dllist *list,
		size_t index);
void	dllist_cat_left(struct s_dllist *dest, struct s_dllist *src);
void	dllist_cat_right(struct s_dllist *dest, struct s_dllist *src);
#endif
