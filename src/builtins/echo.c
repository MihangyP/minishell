/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmihangy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 09:04:51 by pmihangy          #+#    #+#             */
/*   Updated: 2024/10/07 09:33:01 by pmihangy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

bool	has_opt(char *str)
{
	int	i;

	i = 0;
	if (str[0] == '-')
	{
		++i;
		while (str[i])
		{
			if (str[i] != 'n')
				return (false);
			++i;
		}
		return (true);
	}
	return (false);
}

void	echo_minishell(char **argv)
{
	if (!argv)
	{
		printf("\n");
		return ;
	}
	if (has_opt(argv[0]))
		print_argv(argv, true);
	else
		print_argv(argv, false);
}
