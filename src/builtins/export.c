/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmihangy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 10:57:28 by pmihangy          #+#    #+#             */
/*   Updated: 2024/10/09 13:17:23 by pmihangy         ###   ########.fr       */
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

void	verify_if_valide(char *str)
{
	if (str[0] == '\"' || str[0] == '\'')
}

bool	export_minishell(char **env, char **to_export)
{
	int	i;

	if (to_export == NULL)
	{
		env_minishell(env);
		return (true);
	}
	i = 0;
	while (to_export[i])
	{
		if (has_equal(to_export[i]))
		{
			printf("%s\n", to_export[i]);
		}
		else if (has_open_quote(to_export[i], false, 0))
			printf("open quote\n");
		if (!has_equal(to_export[i]))
		{
			verify_if_valid(to_export[i]);
		}
		++i;
	}
	return (true);
}
