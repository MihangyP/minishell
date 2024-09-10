/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmihangy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 10:51:03 by pmihangy          #+#    #+#             */
/*   Updated: 2024/09/10 10:57:20 by pmihangy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	trim_spaces(char *entry, int *cur)
{
	while (entry[*cur] && is_space(entry[*cur]))
		(*cur)++;
	if (entry[*cur] == '\0')
		return (2);
	return (0);
}

int	calc_text_token_len(char *entry, int i)
{
	int	len;

	len = 0;
	while (!is_space(entry[i]) && !is_operator(entry[i]))
	{
		++i;
		++len;
	}
	return (len);
}

int	calc_text_in_quote_len(char *entry, int i, char quote)
{
	int	len;

	len = 0;
	i++;
	while (entry[i] != quote)
	{
		++i;
		++len;
	}
	return (len + 2);
}

int	calc_env_len(char *text, int start)
{
	int	len;

	len = 0;
	while (text[start] && text[start] != '$' && !is_space(text[start]))
	{
		++len;
		++start;
	}
	return (len);
}

int	count_text_size(char *text)
{
	int		i;
	int		size;
	char	*env;

	i = 0;
	size = 0;
	while (text[i])
	{
		if (text[i] != '$')
		{
			++size;
			++i;
		}
		else
		{
			env = getenv(ft_substr(text, i + 1, calc_env_len(text, i + 1)));	
			size += ft_strlen(env);
			i += calc_env_len(text, i + 1) + 1;
		}
	}
	return (size);
}
