/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmihangy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 12:13:11 by pmihangy          #+#    #+#             */
/*   Updated: 2024/10/22 12:58:42 by pmihangy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*get_dir_path(char *cmd, char *path)
{
	char	**arr;
	int		i;

	arr = ft_split(path, ':');
	IF_RETURN(!arr, NULL)
	i = 0;
	while (arr[i])
	{
		if (verif_inside(arr[i], cmd))
			return (ft_strjoin(arr[i], "/"));
		++i;
	}
	return (NULL);
}

bool	repl(char ***env, char *path)
{
	char		*entry;
	t_token		*token_root;
	t_ast		*ast_root;
	char		**builtins;
	pid_t		pid;

	builtins = malloc(8 * sizeof(char *));
	IF_RETURN(!builtins, false)
	IF_RETURN(!insert_builtins(builtins), false)
	while (1)
	{
		entry = readline("minishell> ");
		IF_RETURN(!entry, false)
		add_history(entry);
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
			t_verif	*verify = validate(token_root, path, builtins);
			IF_RETURN(!verify, false)
			if (!verify->res)
				printf("command '%s' not found\n", verify->name);
			else
			{
				// TODO: exec builtins functions
				if (!ast_root->left && !ast_root->right)
				{
					if (is_in(ast_root->text, builtins))
					{
						if (!ft_strncmp(ast_root->text, "echo", 69))
							echo_minishell(ast_root->argv);
						else if (!ft_strncmp(ast_root->text, "env", 69))
						{
							if (ast_root->argv)
								printf("usage: env\n");
							else
								env_minishell(*env);
						}
						else if (!ft_strncmp(ast_root->text, "export", 69))
							export_minishell(env, ast_root->argv);	
						else if (!ft_strncmp(ast_root->text, "unset", 69))
							unset_minishell(env, ast_root->argv);	
						else if (!ft_strncmp(ast_root->text, "pwd", 69))
							pwd_minishell();	
						else if (!ft_strncmp(ast_root->text, "cd", 69))
							cd_minishell(ast_root->argv, env);	
						else if (!ft_strncmp(ast_root->text, "exit", 69))
							exit_minishell(ast_root->argv);	
					}
					else
					{
						pid = fork();
						if (pid == 0)
						{
							if (ft_strchr(ast_root->text, '/'))
								execve(ast_root->text, ft_split(entry, ' '), *env);
							else
							{
								char	*dir_path = get_dir_path(ast_root->text, path);
								IF_RETURN(!dir_path, false)
								execve(ft_strjoin(dir_path, ast_root->text), ft_split(entry, ' '), *env);
							}
						}
						else
							wait(NULL);
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
	IF_RETURN(!repl(&env, path), 1)
	return (0);
}
