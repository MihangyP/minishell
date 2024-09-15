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
	t_token		*token_root;
	t_ast		*ast_root;
	int			i;

	entry = readline("minishell> ");
	if (entry == NULL)
		return (1);
	if (has_open_quote(entry, false, 0))
	{
		printf("open quote\n");
		return (1); // TODO: show an error message like "open quote" 	
	}
	token_root = lexer(entry);
	if (token_root == NULL)
		return (1); // TODO: show an error message like "open quote" 	
	/*print_tokens(token_root);*/
	ast_root = parse(token_root); // TODO: implement the function
	if (ast_root == NULL)
		return (1);
	/*print_ast(ast_root); // TODO: implement the function*/
	printf("%s\n", ast_root->text);
	printf("===ARGUMENTS===\n");
	i = 0;
	while (ast_root->argv[i])
	{
		printf("%s\n", ast_root->argv[i]);
		++i;
	}
	free(entry);
	return (0);
}
