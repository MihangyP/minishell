/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bool.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmihangy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 10:48:18 by pmihangy          #+#    #+#             */
/*   Updated: 2024/11/07 11:48:43 by pmihangy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

bool	is_operator(char c)
{
	return (c == '<' || c == '>' || c == '|' || c == '\0');
}

bool	has_an_env(char *text)
{
	int	i;

	i = 0;
	while (text[i])
	{
		if (text[i] == '$')
			return (true);
		++i;
	}
	return (false);
}
