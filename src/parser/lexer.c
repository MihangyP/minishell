/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmihangy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 12:17:10 by pmihangy          #+#    #+#             */
/*   Updated: 2024/09/11 09:33:37 by pmihangy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	insert_env_in_s(char *text, char *s, int *i, int *j)
{
	char	*env;
	int		t;

	env = getenv(ft_substr(text, *i + 1, calc_env_len(text, *i + 1)));	
	t = 0;
	while (env[t])
	{
		s[*j] = env[t];
		++(*j);
		++t;
	}
	(*i) += calc_env_len(text, *i + 1) + 1;
}
char	*extract_text(char *text)
{
	char	*s;
	int		i;
	int		j;

	s = malloc((count_text_size(text) + 1) * sizeof(char));
	if (s == NULL)
		return (NULL);
	i = 0;
	j = 0;
	while (text[i])
	{
		if (text[i] != '$')
		{
			s[j] = text[i];
			++j;
			++i;
		}
		else
			insert_env_in_s(text, s, &i, &j);
	}
	s[j] = '\0';
	return (s);
}

bool	insert_cmd_token(char *entry, t_token **root, int *i)
{
	char	*text;
	int		token_len;

	token_len = calc_text_token_len(entry, *i);
	text = ft_substr(entry, *i, token_len);
	if (text == NULL)
		return (false);
	tokens_append(root, new_token(text, CMD));
	(*i) += token_len;
	return (true);
}

bool	insert_argument_token(char *entry, t_token **root, int *i)
{
	char	*text;
	int		token_len;

	if (entry[*i] == '\"' || entry[*i] == '\'')
	{
		token_len = calc_text_in_quote_len(entry, *i, entry[*i]);
		text = ft_strtrim(ft_substr(entry, *i, token_len), "\"\'");
		if (entry[*i] == '\"' && has_an_env(text))
			text = extract_text(text);
	}
	else
	{
		token_len = calc_text_token_len(entry, *i);
		text = ft_strtrim(ft_substr(entry, *i, token_len), "\"\'");
		if (text[0] == '$')
			text = getenv(ft_substr(text, 1, ft_strlen(text) - 1));
	}
	if (text == NULL)
		return (false);
	tokens_append(root, new_token(text, ARGUMENT));
	(*i) += token_len;
	return (true);
}

bool	insert_redirections_token(char *entry, t_token **root, int *i)
{
	char	*text;
	char	quote;

	quote = entry[*i];
	if (entry[*i + 1] == quote)
	{
		text = ft_substr(entry, *i, 2);
		if (text == NULL)
			return (false);
		if (entry[*i] == '<')
			tokens_append(root, new_token(text, DOUBLE_LEFT_REDIRECTION));
		else
			tokens_append(root, new_token(text, DOUBLE_RIGHT_REDIRECTION));
		(*i) += 2;
	}
	else
	{
		text = ft_substr(entry, *i, 1);
		if (text == NULL)
			return (false);
		if (entry[*i] == '<')
			tokens_append(root, new_token(text, LEFT_REDIRECTION));
		else
			tokens_append(root, new_token(text, RIGHT_REDIRECTION));
		(*i)++;
	}
	return (true);
}

bool	insert_pipe_token(char *entry, t_token **root, int *i)
{
	char	*text;

	text = ft_substr(entry, *i, 1);
	if (text == NULL)
		return (false);
	tokens_append(root, new_token(text, PIPE));
	(*i)++;
	return (true);
}

t_token	*lexer(char *entry)
{
	t_token	*root;
	int		i;
	int		tmp;

	root = NULL;
	i = 0;
	if (trim_spaces(entry, &i) == 2)
		return (NULL);
	tmp = i;
	while (entry[i])
	{
		if (i == tmp && !insert_cmd_token(entry, &root, &i))
				return (NULL);
		if (!entry[i])
			return (root);
		if (trim_spaces(entry, &i) == 2)
			return (root);
		if (!is_operator(entry[i]) && tokens_find_last(root)->text[0] == '|') // if the last token is an pipe, the following is a CMD
		{
			if (!insert_cmd_token(entry, &root, &i))
				return (NULL);
		}
		else if (!is_operator(entry[i]) && !insert_argument_token(entry, &root, &i)) // if it is an argument
			return (NULL);
		else if (is_operator(entry[i]) && (entry[i] == '<' || entry[i] == '>')) // if it is an operator
		{
			if (!insert_redirections_token(entry, &root, &i))
				return (NULL);
		}
		else if (is_operator(entry[i]) && entry[i] == '|') // if it is an operator
		{
			if (!insert_pipe_token(entry, &root, &i))
				return (NULL);
		}
	}
	return (root);
}
