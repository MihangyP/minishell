/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmihangy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 12:31:41 by pmihangy          #+#    #+#             */
/*   Updated: 2024/09/17 11:53:46 by pmihangy         ###   ########.fr       */
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

