/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmihangy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 12:13:11 by pmihangy          #+#    #+#             */
/*   Updated: 2024/09/07 13:32:55 by pmihangy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	main(void)
{
	char		*entry;
	t_token	*root;

	entry = readline("minishell> ");
	if (entry == NULL)
		return (1);
	if (has_open_quote(entry, false, 0))
	{
		printf("open quote\n");
		return (1); // TODO: break the REPL and show an error message like "open quote" 	
	}
	root = lexer(entry);
	if (root == NULL)
		return (1); // TODO: break the REPL and show an error message like "open quote" 	
	print_tokens(root);
	free(entry);
	return (0);
}
