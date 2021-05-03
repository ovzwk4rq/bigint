# include <array_list.h>
# include <stdint.h>

struct s_u64_array_node
{
	struct s_dllist_node	node;
	uint64_t				array[AL_CHUNK];
};

struct s_bigint
{
	struct s_array_list	abs;
	int					sign;
};

enum e_sign
{
	PLUS,
	MINUS
};
