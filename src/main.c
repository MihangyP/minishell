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

bool	insert_builtins(char **builtins)
{
	builtins[0] = ft_strdup("echo");
	IF_RETURN(!builtins[0], false)
	builtins[1] = ft_strdup("cd");
	IF_RETURN(!builtins[1], false)
	builtins[2] = ft_strdup("pwd");
	IF_RETURN(!builtins[2], false)
	builtins[3] = ft_strdup("export");
	IF_RETURN(!builtins[3], false)
	builtins[4] = ft_strdup("unset");
	IF_RETURN(!builtins[4], false)
	builtins[5] = ft_strdup("env");
	IF_RETURN(!builtins[5], false)
	builtins[6] = ft_strdup("exit");
	IF_RETURN(!builtins[6], false)
	builtins[7] = NULL;
	return (true);
}

bool	verif_inside(char *directory, char *cmd)
{
	DIR				*dirp;
	struct dirent	*dirr;

	dirp = opendir(directory);
	IF_RETURN(!dirp, false)
	dirr = readdir(dirp);
	while (dirr)
	{
		IF_RETURN(!ft_strncmp(cmd, dirr->d_name, 69), true)
		dirr = readdir(dirp);
	}
	closedir(dirp);
	return (false);
}

size_t	verify_if_in_path(char *cmd, char *path)
{
	char	**bins_dir;
	char		**tmp;
	int			i;

	bins_dir = ft_split(path, ':');
	IF_RETURN(!bins_dir, 2)
	i = 0;
	while (bins_dir[i])
	{
		IF_RETURN(verif_inside(bins_dir[i], cmd), 1)
		++i;
	}
	return (0);
}

bool	inside_builtins(char *cmd, char **builtins)
{
	int	i;

	i = 0;
	while (builtins[i])
	{
		IF_RETURN(!ft_strncmp(cmd, builtins[i], 69), true)
		++i;
	}
	return (false);
}

t_verif	*validate(t_token *token_root, char *path, char **builtins)
{
	t_verif	*result;

	result = malloc(sizeof(t_verif));
	IF_RETURN(!result, NULL)
	result->res = true;
	result->name = NULL;
	while (token_root)
	{
		if (token_root->identifier == CMD)
		{
			if (!inside_builtins(token_root->text, builtins))
			{
				if (!verify_if_in_path(token_root->text, path))
				{
					result->res = false;
					result->name = ft_strdup(token_root->text);
					IF_RETURN(!result->name, NULL)
					return (result);
				}
				else if (verify_if_in_path(token_root->text, path) == 2)
					return (NULL);
			}
		}
		token_root = token_root->next;
	}
	return (result);
}

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

char	*get_path(char **env)
{
	int	i;

	i = 0;
	while (ft_strncmp(env[i], "PATH", 4))
		++i;
	return (env[i] + 5);
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
