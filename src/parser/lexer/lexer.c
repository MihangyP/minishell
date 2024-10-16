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

/*
 * This function create a simple redirection token
 * and add it in the list of tokens
 */
bool	insert_simple_redirections(char *entry, t_token **root, int *i)
{
	char	*text;

	text = ft_substr(entry, *i, 1);
	IF_RETURN(!text, false)
	if (entry[*i] == '<')
		tokens_append(root, new_token(text, LEFT_REDIRECTION));
	else
		tokens_append(root, new_token(text, RIGHT_REDIRECTION));
	(*i)++;
	return (true);
}

/*
 * This function create a redirection token
 * and add it in the list of tokens
 *
 * It create a heredoc token if we have a double redirection
 * Else we call insert_simple_redirections function
 */
bool	insert_redirections_token(char *entry, t_token **root, int *i)
{
	char	*text;
	char	quote;

	quote = entry[*i];
	if (entry[*i + 1] == quote)
	{
		text = ft_substr(entry, *i, 2);
		IF_RETURN(!text, false)
		if (entry[*i] == '<')
			tokens_append(root, new_token(text, DOUBLE_LEFT_REDIRECTION));
		else
			tokens_append(root, new_token(text, DOUBLE_RIGHT_REDIRECTION));
		(*i) += 2;
	}
	else if (!insert_simple_redirections(entry, root, i))
		return (false);
	return (true);
}

/*
 * This function create a pipe token
 * and append it in the list of tokens
 */
bool	insert_pipe_token(char *entry, t_token **root, int *i)
{
	char	*text;

	text = ft_substr(entry, *i, 1);
	IF_RETURN(!text, false)
	tokens_append(root, new_token(text, PIPE));
	(*i)++;
	return (true);
}

/* 
 * This function is called in lexer function
 * It create the others tokens and append in the list
 * 
 * If the last token is a pipe, the following is a CMD, so we create a cmd token
 * Else if it is an argument, we create a argument token
 * Else if it is a redirection, we create a redirection cmd
 * Else if it is a pipe, we create a pipe token
 *
 */
bool	append_tokens(char *entry, t_token **root, int *i)
{
	if (!is_operator(entry[*i]) && tokens_find_last(*root)->text[0] == '|')
	{
		IF_RETURN(!insert_cmd_token(entry, root, i), false)
	}
	else if (!is_operator(entry[*i]) && !insert_argument_token(entry, root, i))
		return (false);
	else if (is_operator(entry[*i]) && (entry[*i] == '<' || entry[*i] == '>'))
	{
		IF_RETURN(!insert_redirections_token(entry, root, i), false)
	}
	else if (is_operator(entry[*i]) && entry[*i] == '|')
	{
		IF_RETURN(!insert_pipe_token(entry, root, i), false)
	}
	return (true);
}

/*
 * This function divide the entry into a list of tokens
 * A token have two element : text && identifier
 * 
 * typedef struct s_token
 * {
 *		char			*text;
 *		t_identifier	identifier;
 * }	t_token;
 * 
 * Fist we trim the spaces in begin if we have spaces
 * Then if we are in the beginning, we create a cmd token (TODO: to verify if it's really the case)
 * Then we trim spaces again
 * The we call append_tokens function who create and append others tokens
 */
t_token	*lexer(char *entry)
{
	t_token	*root;
	int		i;
	int		tmp;

	root = NULL;
	i = 0;
	IF_RETURN(trim_spaces(entry, &i) == 2, NULL)
	tmp = i;
	while (entry[i])
	{
		if (i == tmp && !insert_cmd_token(entry, &root, &i))
				return (NULL);
		IF_RETURN(!entry[i], root)
		IF_RETURN(trim_spaces(entry, &i) == 2, root)
		IF_RETURN(!append_tokens(entry, &root, &i), NULL)
	}
	return (root);
}
