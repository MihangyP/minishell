/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_manip.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmihangy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 10:46:41 by pmihangy          #+#    #+#             */
/*   Updated: 2024/09/17 11:28:24 by pmihangy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_token	*new_token(char *text, t_identifier identifier)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (new == NULL)
		return (NULL);
	new->text = text;
	new->identifier = identifier;
	new->prev = NULL;
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

void	tokens_append(t_token **root, t_token *new_el)
{
	t_token	*last;

	if (*root == NULL)
	{
		*root = new_el;
		return ;
	}
	last = tokens_find_last(*root);
	last->next = new_el;
	new_el->prev = last;
}

size_t	list_size(t_token *token)
{
	size_t	size;

	size = 0;
	if (token == NULL)
		return (0);
	while (token)
	{
		++size;
		token = token->next;
	}
	return (size);
}
