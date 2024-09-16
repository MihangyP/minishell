/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmihangy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 12:31:41 by pmihangy          #+#    #+#             */
/*   Updated: 2024/09/16 10:14:01 by pmihangy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

bool	is_space(char c)
{
	return ((c >= 8 && c <= 13) || c == 32);
}

void	print_tokens(t_token *root)
{
	t_token	*curr;

	curr = root;
	while (curr)
	{
		printf("text: %s, identifier: %d\n", curr->text, curr->identifier);
		curr = curr->next;
	}
}

void	print_ast(t_ast	*ast_root)
{
	printf("Print ast\n");
}
