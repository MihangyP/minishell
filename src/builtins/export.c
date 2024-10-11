/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmihangy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 10:57:28 by pmihangy          #+#    #+#             */
/*   Updated: 2024/10/11 16:11:44 by pmihangy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>


bool	has_equal(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
			return (true);
		++i;
	}
	return (false);
}

/*
 * TODO: cases to manage
 *
 * export DONTO => do nothing
 * export "'DONTO'" => show mess error
 * export "'DONTO'"=DONTO show mess error
 * export DONTO=DONTO => insert it in env
 * export "DONTO"=DONTO => insert DONTO=DONTO in env
 * export "DONTO=DONTO => show error mess
 * export DONTO="DONTO" => insert DONTO=DONTO in env
 * export DONTO="'DONTO'" => insert DONTO='DONTO' in env
 *
 */

size_t	calc_size_str_trimed(char *str)
{
	size_t	counter;

	counter = 0;

}

void	verify_if_valid(char *str)
{
	char	*str_trimed;
	size_t	size;

	if (str[i] == '\'' || str[i] == '\"')
	{
		size = calc_size_str_trimed(str);
	}
}

bool	export_minishell(char **env, char **to_export)
{
	int	i;
	char	*s_trimed;

	if (to_export == NULL)
	{
		env_minishell(env);
		return (true);
	}
	i = 0;
	while (to_export[i])
	{
		// TODO: manage errors
		if (has_open_quote(to_export[i], false, 0))
		{
			printf("open quote\n");
			return (true);
		}
		if (has_equal())
		++i;
	}
	return (true);
}
