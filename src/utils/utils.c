/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmihangy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 12:31:41 by pmihangy          #+#    #+#             */
/*   Updated: 2024/11/07 13:57:17 by pmihangy         ###   ########.fr       */
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
		printf("text: %s, id: %d\n", curr->text, curr->id);
		curr = curr->next;
	}
}

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

char	*get_path(char **env)
{
	int	i;

	i = 0;
	while (ft_strncmp(env[i], "PATH", 4))
		++i;
	return (env[i] + 5);
}

char	*get_dir_path(char *cmd, char *path)
{
	char	**arr;
	int		i;

	arr = ft_split(path, ':');
	if (!arr)
		return (NULL);
	i = 0;
	while (arr[i])
	{
		if (verif_inside(arr[i], cmd))
			return (ft_strjoin(arr[i], "/"));
		++i;
	}
	return (NULL);
}

void	error(const char *err_message)
{
	printf("ERROR: %s\n", err_message);
	exit(1);
}
