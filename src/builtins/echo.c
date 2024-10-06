#include <minishell.h>

static void	print_argv(char **argv, bool has_opt)
{
	int	i;

	i = 0;
	if (has_opt)
		++i;
	while (argv[i])
	{
		if (!argv[i + 1])
			printf("%s", argv[i]);
		else
			printf("%s ", argv[i]);
		++i;
	}
	if (!has_opt)
		printf("\n");
}

void	echo(char **argv)
{
	if (!argv)
	{
		printf("\n");
		return ;
	}
	if (!ft_strncmp(argv[0], "-n", 69))
		print_argv(argv, true);
	else
		print_argv(argv, false);
}
