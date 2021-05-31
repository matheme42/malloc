#include "malloc.h"

int main(void)
{
	int		i;
	size_t	tab[1024];

	i = 0;
	while (i < 199)
	{
		tab[i] = (size_t)malloc(120);
		i++;
	}
	tab[i] = (size_t)malloc(16);

	char *a = malloc(16);

	i = 0;
	while (i < 199)
	{
		free((void*)tab[i]);
		i++;
	}

	free((void*)tab[i]);
	free(a);
	free(a);
	show_alloc_mem();
	return (0);
}
