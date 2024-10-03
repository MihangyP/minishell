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

bool	insert_builtins(char **builtins)
{
	builtins[0] = ft_strdup("echo");
	if (builtins[0] == NULL)
		return (false);
	builtins[1] = ft_strdup("cd");
	if (builtins[1] == NULL)
		return (false);
	builtins[2] = ft_strdup("pwd");
	if (builtins[2] == NULL)
		return (false);
	builtins[3] = ft_strdup("export");
	if (builtins[3] == NULL)
		return (false);
	builtins[4] = ft_strdup("unset");
	if (builtins[4] == NULL)
		return (false);
	builtins[5] = ft_strdup("env");
	if (builtins[5] == NULL)
		return (false);
	builtins[6] = ft_strdup("exit");
	if (builtins[6] == NULL)
		return (false);
	builtins[7] = NULL;
	return (true);
}

bool	verif_inside(char *directory, char *cmd)
{
	DIR				*dirp;
	struct dirent	*dirr;

	dirp = opendir(directory);
	if (dirp == NULL)
		return (false);
	dirr = readdir(dirp);
	while (dirr)
	{
		if (!ft_strncmp(cmd, dirr->d_name, 69))
			return (true);
		dirr = readdir(dirp);
	}
	closedir(dirp);
	return (false);
}

t_verif *verify_if_in_path(char *cmd, char *path)
{
	t_verif			*result;
	char			**bins_dir;
	char			**tmp;
	int				i;

	result = malloc(sizeof(t_verif));
	if (result == NULL)
		return (NULL);
	bins_dir = ft_split(path, ':');
	if (bins_dir == NULL)
		return (NULL);
	tmp = ft_split(bins_dir[0], '=');
	if (tmp == NULL)
		return (NULL);
	bins_dir[0] = tmp[1]; 
	i = 0;
	while (bins_dir[i])
	{
		if (verif_inside(bins_dir[i], cmd) == true)
		{
			result->res = true;
			result->name = NULL;
			return (result);
		}
		++i;
	}
	result->res = false;
	result->name = ft_strdup(cmd);
	if (result->name == NULL)
		return (NULL);
	return (result);
}

bool	inside_builtins(char *cmd, char **builtins)
{
	int	i;

	i = 0;
	while (builtins[i])
	{
		if (!ft_strncmp(cmd, builtins[i], 69))
			return (true);
		++i;
	}
	return (false);
}

// TODO: find a way to iterate over the ast from the very left node
t_verif	*verify_recursively(t_ast *left_node, char **builtins, char *path)
{
	t_verif	*result;	

	result = malloc(sizeof(t_verif));
	if (result == NULL)
		return (NULL);
	result->res = false;
	result->name = ft_strdup(left_node->text);
	if (result->name == NULL)
		return (NULL);
	if (left_node->identifier == CMD)
	{
		// TODO:  verify if in builtins
		if (inside_builtins(left_node->text, builtins))	
		{
			result->res = true;
			result->name = NULL;
			return (result);
		}
		// TODO: verify if in PATH binaries
		result = verify_in_path(left_node->text, path);
		if (result == NULL)
			return (NULL);
	}
	return (result);
}

t_verif	*validate(t_ast *ast, char *path)
{
	t_verif	*result;
	char	**builtins;
	t_ast	*left_node;
	int		i;

	builtins = malloc(8 * sizeof(char *));
	if (!builtins)
		return (NULL);
	result = malloc(sizeof(t_verif));
	if (!result)
		return (NULL);
	if (!insert_builtins(builtins))	
		return (NULL);
	i = 0;
	if (!ast->left && !ast->right)
	{
		while (builtins[i])
		{
			if (!ft_strncmp(ast->text, builtins[i], ft_strlen(ast->text)))
			{
				result->res = true;
				result->name = NULL;
				return (result);
			}	
			++i;
		}
		result = verify_if_in_path(ast->text, path);
		if (result == NULL)
			return (NULL);
	}
	else
	{
		// TODO: implement this function
		left_node = ast;
		while (left_node->left)
			left_node = left_node->left;
		result = verify_recursively(left_node, builtins, path);
		if (result == NULL)
			return (NULL);
	}
	return (result);
}

bool	repl(char *path)
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
			/*print_ast(ast_root, 0);*/
			t_verif	*verify = validate(ast_root, path);
			if (verify == NULL)
				return (false);
			if (!verify->res)
				printf("command '%s' not found\n", verify->name);
		}
		free(entry);
	}
	return (true);
}

int	main(int ac, char **av, char **env)
{
	char	*path;

	int i = 0;
	while (ft_strncmp(env[i], "PATH", 4))
		++i;
	path = ft_strdup(env[i]);
	if (path == NULL)
		return (1);
	if (!repl(path))
		return (1);
	return (0);
}
