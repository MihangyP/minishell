/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmihangy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 12:13:11 by pmihangy          #+#    #+#             */
/*   Updated: 2024/10/15 12:22:20 by pmihangy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

bool	repl(char **env, char *path)
{
	char		*entry;
	t_token		*token_root;
	t_ast		*ast_root;
	char		**builtins;

	builtins = malloc(8 * sizeof(char *));
	IF_RETURN(!builtins, NULL)
	IF_RETURN(!insert_builtins(builtins), NULL)
	while (1)
	{
		entry = readline("minishell> ");
		IF_RETURN(!entry, false)
		if (has_open_quote(entry, false, 0))
			printf("open quote\n");
		else
		{
			token_root = lexer(entry);
			if (token_root == NULL)
				return (free(entry), false);
			print_tokens(token_root);
			ast_root = parse(token_root);
			if (ast_root == NULL)
				return (free(entry), false);
			t_verif	*verify = validate(token_root, path, builtins);
			IF_RETURN(!verify, false)
			if (!verify->res)
				printf("command '%s' not found\n", verify->name);
			else
			{
				// TODO: exec builtins functions
				if (!ast_root->left && !ast_root->right)
				{
					if (inside_builtins(ast_root->text, builtins))
					{
						if (!ft_strncmp(ast_root->text, "echo", 69))
							echo_minishell(ast_root->argv);
						else if (!ft_strncmp(ast_root->text, "env", 69))
						{
							if (ast_root->argv)
								printf("usage: env\n");
							else
								env_minishell(env);
						}
						else if (!ft_strncmp(ast_root->text, "export", 69))
							export_minishell(env, ast_root->argv);	
					}
				}
			}
		}
		free(entry);
	}
	return (true);
}

// TODO
// Rethink our approach
//
// Our approach
// - BUILD a TOKENIZER (TODO: improve and refactor)
// - BUILD a AST
// - EXEC BY TRAVERSING THE AST
int	main(int ac, char **av, char **env)
{
	int		i;
	char	*path;

	path = get_path(env);
	IF_RETURN(!repl(env, path), 1)
	return (0);
}
