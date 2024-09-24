/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmihangy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 12:13:11 by pmihangy          #+#    #+#             */
/*   Updated: 2024/09/24 10:04:18 by pmihangy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	print_ast(t_ast *ast, int indent)
{
	int	i;

	i = 0;
	if (!ast)
		return ;
	while (i < indent)
	{
		printf("	");
		i++;
	}
	printf("%s ", ast->text);
	if (ast->argv)
	{
		for (int i = 0; ast->argv[i] != NULL; ++i)
			printf("%s ", ast->argv[i]);
	}
	printf("\n");
	print_ast(ast->left, indent + 1);
	print_ast(ast->right, indent + 1);
}

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
		return (1);
	}
	token_root = lexer(entry);
	if (token_root == NULL)
		return (1);
	/*print_tokens(token_root);*/
	ast_root = parse(token_root);
	if (ast_root == NULL)
		return (1);
	print_ast(ast_root, 0);
	free(entry);
	return (0);
}
