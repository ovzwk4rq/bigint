#include <dllist.h>

static inline void	link(
		struct s_dllist_node *left,
		struct s_dllist_node *right)
{
	left->right = right;
	right->left = left;
}

void	dllist_init(
		struct s_dllist *list,
		void (*free_node)(void *))
{
	link(&list->entry, &list->entry);
	list->size = 0;
	list->free_node = free_node;
}

static inline void	link_new_between(
		struct s_dllist_node *new,
		struct s_dllist_node *left,
		struct s_dllist_node *right)
{
	link(left, new);
	link(new, right);
}

void	dllist_link_left(struct s_dllist *list, struct s_dllist_node *new)
{
	link_new_between(new, &list->entry, list->entry.right);
	list->size++;
}

void	dllist_link_right(struct s_dllist *list, struct s_dllist_node *new)
{
	link_new_between(new, list->entry.left, &list->entry);
	list->size++;
}

static struct s_dllist_node	*free_and_get_left(
		struct s_dllist_node *node,
		void (*free_node)(void *))
{
	struct s_dllist_node	*next;

	next = node->left;
	free_node(node);
	return (next);
}

static struct s_dllist_node	*free_and_get_right(
		struct s_dllist_node *node,
		void (*free_node)(void *))
{
	struct s_dllist_node	*next;

	next = node->right;
	free_node(node);
	return (next);
}

static void	delete_left_from(
		struct s_dllist_node *curr,
		size_t n,
		void (*free_node)(void *))
{
	struct s_dllist_node	*right;

	right = curr->right;
	while (n-- > 0)
		curr = free_and_get_left(curr, free_node);
	link(curr, right);
}

static void	delete_right_from(
		struct s_dllist_node *curr,
		size_t n,
		void (*free_node)(void *))
{
	struct s_dllist_node	*left;

	left = curr->left;
	while (n-- > 0)
		curr = free_and_get_right(curr, free_node);
	link(left, curr);
}

int	dllist_delete_left(struct s_dllist *list, size_t n)
{
	if (list->size < n)
		return (0);
	delete_right_from(list->entry.right, n, list->free_node);
	list->size -= n;
	return (1);
}

int	dllist_delete_right(struct s_dllist *list, size_t n)
{
	if (list->size < n)
		return (0);
	delete_left_from(list->entry.left, n, list->free_node);
	list->size -= n;
	return (1);
}

static size_t	delete_all_between(
		struct s_dllist_node *left,
		struct s_dllist_node *right,
		void (*free_node)(void *))
{
	struct s_dllist_node	*curr;
	size_t					count;

	count = 0;
	curr = left->right;
	while (curr != right)
	{
		curr = free_and_get_right(curr, free_node);
		count++;
	}
	link(left, right);
	return (count);
}

void	dllist_delete_left_from(
		struct s_dllist *list,
		struct s_dllist_node *curr)
{
	if (!curr)
		return ;
	list->size -= delete_all_between(
			&list->entry,
			curr->right,
			list->free_node);
}

void	dllist_delete_right_from(
		struct s_dllist *list,
		struct s_dllist_node *curr)
{
	if (!curr)
		return ;
	list->size -= delete_all_between(
			curr->left,
			&list->entry,
			list->free_node);
}

void	dllist_delete_all(struct s_dllist *list)
{
	list->size -= delete_all_between(
			&list->entry,
			&list->entry,
			list->free_node);
}

struct s_dllist_node	*dllist_get_left_end(struct s_dllist *list)
{
	if (list->entry.right == &list->entry)
		return (0);
	return (list->entry.right);
}

struct s_dllist_node	*dllist_get_right_end(struct s_dllist *list)
{
	if (list->entry.left == &list->entry)
		return (0);
	return (list->entry.left);
}

struct s_dllist_node	*dllist_get_left(
		struct s_dllist *list,
		struct s_dllist_node *curr,
		size_t n)
{
	while (n-- > 0)
	{
		curr = curr->left;
		if (curr == &list->entry)
			return (0);
	}
	return (curr);
}

struct s_dllist_node	*dllist_get_right(
		struct s_dllist *list,
		struct s_dllist_node *curr,
		size_t n)
{
	while (n-- > 0)
	{
		curr = curr->right;
		if (curr == &list->entry)
			return (0);
	}
	return (curr);
}

struct s_dllist_node	*dllist_get_nth_from_left(
		struct s_dllist *list,
		size_t index)
{
	return (dllist_get_right(list, &list->entry, index + 1));
}

struct s_dllist_node	*dllist_get_nth_from_right(
		struct s_dllist *list,
		size_t index)
{
	return (dllist_get_left(list, &list->entry, index + 1));
}

static inline void	add_list_between(
		struct s_dllist *list,
		struct s_dllist_node *left,
		struct s_dllist_node *right)
{
	link(left, list->entry.right);
	link(list->entry.left, right);
}

void	dllist_cat_left(struct s_dllist *dest, struct s_dllist *src)
{
	add_list_between(src, &dest->entry, dest->entry.right);
	dest->size += src->size;
	dllist_init(src, src->free_node);
}

void	dllist_cat_right(struct s_dllist *dest, struct s_dllist *src)
{
	add_list_between(src, dest->entry.left, &dest->entry);
	dest->size += src->size;
	dllist_init(src, src->free_node);
}
