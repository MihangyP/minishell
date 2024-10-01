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

bool	validate(t_ast *ast)
{
	
}

bool	repl(void)
{
	char		*entry;
	t_token		*token_root;
	t_ast		*ast_root;
	int			i;

	while (1)
	{
		entry = readline("minishell> ");
		if (entry == NULL)
			return (false);
		if (has_open_quote(entry, false, 0))
			printf("open quote\n");
		else
		{
			token_root = lexer(entry);
			if (token_root == NULL)
				return (free(entry), false);
			ast_root = parse(token_root);
			if (ast_root == NULL)
				return (free(entry), false);
			t_verif	verify = validate(ast_root);
			if (!verify.res)
				printf("comand not found: %s\n", verify.name);
			else
			{
				printf("AST validated !\n");
			}
		}
		free(entry);
	}
	return (true);
}

// TODO: gerer les signals: Ctrl-C, Ctrl-D, Ctrl-

int	main(void)
{
	if (!repl())
		return (1);
	return (0);
}
