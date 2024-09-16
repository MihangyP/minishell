#include <stdio.h>

int main(int ac, char **av)
{
	int i;

	if (ac >= 2)
	{
		i = 1;
		while (av[i])
		{
			if (av[i + 1] == NULL)
				printf("%s", av[i]);
			else
				printf("%s ", av[i]);
			++i;
		} 
		printf("\n");
	}
	return (0);
}
