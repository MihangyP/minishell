/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmihangy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 12:17:10 by pmihangy          #+#    #+#             */
/*   Updated: 2024/09/07 16:40:22 by pmihangy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	trim_spaces(char *entry, int *cur)
{
	while (entry[*cur] && is_space(entry[*cur]))
		(*cur)++;
	if (entry[*cur] == '\0')
		return (2); // 2 to indicate that we are in the end of the string
	return (0);
}

bool	is_operator(char c)
{
	return (c == '<' || c == '>' || c == '|' || c == '\0');
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

t_token	*new_token(char *text, t_identifier identifier)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (new == NULL)
		return (NULL);
	new->text = text;
	new->identifier = identifier;
	new->next = NULL;
	return (new);
}

t_token	*tokens_find_last(t_token *root)
{
	t_token	*curr;

	curr = root;
	while (curr->next)
		curr = curr->next;
	return (curr);
}

void	tokens_append(t_token **root, t_token *new)
{
	t_token	*last;

	if (*root == NULL)
	{
		*root = new;
		return ;
	}
	last = tokens_find_last(*root);
	last->next = new;
}

// echo "Hello World" > donto.txt 
// TODO: refactor this function
// Manage string in a quotes

t_token	*lexer(char *entry)
{
	t_token	*root;
	int		i;
	int		tmp;
	int		token_len;
	char	*text;

	root = NULL;
	i = 0;
	if (trim_spaces(entry, &i) == 2)
		return (NULL);
	tmp = i;
	while (entry[i])
	{
		if (i == tmp)
		{
			token_len = calc_text_token_len(entry, i);
			text = ft_substr(entry, i, token_len);
			if (text == NULL)
				return (NULL);
			tokens_append(&root, new_token(text, CMD));
			i += token_len;
		}
		if (!entry[i])
			return (root);
		if (trim_spaces(entry, &i) == 2)
			return (root);
		if (!is_operator(entry[i]) && tokens_find_last(root)->text[0] == '|') // if the last token is an pipe, the following is a CMD
		{
			token_len = calc_text_token_len(entry, i);
			text = ft_substr(entry, i, token_len);
			if (text == NULL)
				return (NULL);
			tokens_append(&root, new_token(text, CMD));
			i += token_len;
		}
		else if (!is_operator(entry[i])) // if it is an argument
		{
			if (entry[i] == '\"' || entry[i] == '\'')
				token_len = calc_text_in_quote_len(entry, i, entry[i]);
			else
				token_len = calc_text_token_len(entry, i);
			text = ft_strtrim(ft_substr(entry, i, token_len), "\"\'");	
			if (text == NULL)
				return (NULL);
			tokens_append(&root, new_token(text, ARGUMENT));
			i += token_len;
		}
		else if (is_operator(entry[i])) // if it is an operator
		{
			if (entry[i] == '<')
			{
				if (entry[i + 1] == '<')
				{
					text = ft_substr(entry, i, 2);
					if (text == NULL)
						return (NULL);
					tokens_append(&root, new_token(text, DOUBLE_LEFT_REDIRECTION));
					i += 2;
				}
				else
				{
					text = ft_substr(entry, i, 1);
					if (text == NULL)
						return (NULL);
					tokens_append(&root, new_token(text, LEFT_REDIRECTION));
					i++;
				}
			}
			else if (entry[i] == '>')
			{
				if (entry[i + 1] == '>')
				{
					text = ft_substr(entry, i, 2);
					if (text == NULL)
						return (NULL);
					tokens_append(&root, new_token(text, DOUBLE_RIGHT_REDIRECTION));
					i += 2;
				}
				else
				{
					text = ft_substr(entry, i, 1);
					if (text == NULL)
						return (NULL);
					tokens_append(&root, new_token(text, RIGHT_REDIRECTION));
					i++;
				}
			}
			else
			{
				text = ft_substr(entry, i, 1);
				if (text == NULL)
					return (NULL);
				tokens_append(&root, new_token(text, PIPE));
				i++;
			}
		}
	}
	return (root);
}
