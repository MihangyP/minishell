/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmihangy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 10:51:03 by pmihangy          #+#    #+#             */
/*   Updated: 2024/09/18 13:34:57 by pmihangy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

/*
 * This function trim spaces like his name inicates
 */
int	trim_spaces(char *entry, int *cur)
{
	while (entry[*cur] && is_space(entry[*cur]))
		(*cur)++;
	IF_RETURN(entry[*cur] == '\0', 2)
	return (0);
}

/*
 * This function calculate the length of a token text
 */
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

/*
 * This function calculate the text inside quotes length
 * (quotes included)
 */
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

/*
 * This function calculate the length of env variable
 */
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

/*
 * This function the length of a text inside ""
 * if we extend all the env variables
 */
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
