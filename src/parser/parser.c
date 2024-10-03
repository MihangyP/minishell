/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmihangy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 09:15:59 by pmihangy          #+#    #+#             */
/*   Updated: 2024/09/27 13:56:14 by pmihangy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

bool	has_operator(t_token *token)
{
	while (token)
	{
		if (is_operator(token->text[0]))
			return (true);
		token = token->next;
	}
	return (false);
}

bool	has_operator_with_pipe(t_token *token)
{
	while (token && token->identifier != PIPE)
	{
		if (is_operator(token->text[0]))
			return (true);
		token = token->next;
	}
	return (false);
}

t_ast	*create_cmd(t_token *token_root)
{
	t_ast	*ast;
	int	size;
	int	i;

	ast = malloc(sizeof(t_ast));
	if (ast == NULL)
		return (NULL);
	ast->text = token_root->text;
	ast->identifier = token_root->identifier;
	ast->left = NULL;
	ast->right = NULL;
	ast->parent = NULL;
	token_root = token_root->next;
	if (token_root)
	{
		ast->argv = malloc((list_size(token_root) + 2) * sizeof(char *));
		if (ast->argv == NULL)
			return (NULL);
		ast->argv[0] = ft_strdup(ast->text);
		if (ast->argv[0] == NULL)
			return (NULL);
	}
	i = 1;
	while (token_root)
	{
		ast->argv[i] = ft_strdup(token_root->text);
		if (ast->argv[i] == NULL)
			return (NULL);
		token_root = token_root->next;
		i++;
	}
	if (token_root)
		ast->argv[i] = NULL;
	return (ast);
}

bool	has_pipe(t_token *t)
{
	while (t)
	{
		if (t->text[0] == '|')
			return (true);
		t = t->prev;
	}
	return (false);
}

t_ast	*new_node(t_token *token)
{
	t_ast	*new;

	new = malloc(sizeof(t_ast));
	if (new == NULL)
		return (NULL);
	new->text = token->text;
	new->identifier = token->identifier;
	new->argv = NULL;
	new->left = NULL;
	new->right = NULL;
	new->parent = NULL;
	return (new);
}

t_token	*find_next_ast_node(t_token *last)
{
	t_token	*tmp;

	while (last && !is_operator(last->text[0]))
	{
		tmp = last;
		last = last->prev;
	}
	if (!last)
		return (tmp);
	return (last);
}

// TODO: implement this
t_token	*find_next_ast_node_with_pipe(t_token *last)
{
	t_token	*tmp;

	while (last && last->identifier != PIPE && !is_operator(last->text[0]))
	{
		tmp = last;
		last = last->prev;
	}
	if (last->identifier == PIPE || !last)
		return (tmp);
	return (last);
}

t_token	*find_next_pipe_or_cmd(t_token *token)
{
	t_token	*tmp;

	while (token && token->identifier != PIPE)
	{
		tmp = token;
		token = token->prev;
	}
	if (!token)
		return (tmp);
	return (token);
}

char	**insert_argv(t_token *last)
{
	char	**argv;
	int		size;
	int		i;
	t_token	*curr;

	size = 0;
	curr = last;
	while (curr && !is_operator(curr->text[0]))
	{
		++size;
		curr = curr->next;
	}
	argv = malloc((size + 1) * sizeof(char *));
	if (argv == NULL)
		return (NULL);
	curr = last;
	i = 0;
	while (curr && !is_operator(curr->text[0]))
	{
		argv[i] = ft_strdup(curr->text);
		if (argv[i] == NULL)
			return (NULL);
		++i;
		curr = curr->next;
	}
	argv[i] = NULL;
	return (argv);
}

size_t	count_nb_operators(t_token *last)
{
	size_t	counter;

	counter = 0;
	while (last)
	{
		if (is_operator(last->text[0]))
			++counter;
		last = last->prev;
	}
	return (counter);
}

size_t	count_nb_pipes(t_token *token)
{
	size_t	counter;

	counter = 0;
	while (token)
	{
		if (token->identifier == PIPE)	
			++counter;
		token = token->prev;
	}
	return (counter);
}

size_t	count_nb_operators_with_pipe(t_token *tmp)
{
	size_t	counter;

	counter = 0;
	while (tmp && tmp->identifier != PIPE)
	{
		if (is_operator(tmp->text[0]))
			++counter;
		tmp = tmp->prev;
	}
	return (counter);
}

bool	fill_left(t_ast **ast, t_token *last)
{
	size_t	nb_operators;

	nb_operators = count_nb_operators(last);
	while (nb_operators)
	{
		last = find_next_ast_node(last->prev);
		(*ast)->left = new_node(last);
		(*ast)->left->parent = malloc(sizeof(t_ast));
		if ((*ast)->left->parent == NULL)
			return (false);
		(*ast)->left->parent = *ast;
		if ((*ast)->left->identifier == CMD && last->next->identifier == ARGUMENT)
		{
			(*ast)->left->argv = insert_argv(last->next);
			if ((*ast)->left->argv == NULL)
				return (false);
		}
		else if (is_operator(last->text[0]) && !is_operator(last->next->text[0]))
		{
			(*ast)->left->right = new_node(last->next); 		
			(*ast)->left->right->parent = malloc(sizeof(t_ast));
			if ((*ast)->left->right->parent == NULL)
				return (false);
			(*ast)->left->right->parent = (*ast)->left;
		}
		ast = &((*ast)->left);
		--nb_operators;
	}
	return (true);
}

bool	has_operator_inside_pipe(t_token *token)
{
	while (token && token->identifier != PIPE)
	{
		if (is_operator(token->text[0]))
			return (true);
		token = token->next;
	}
	return (false);
}

t_token	*find_operator_inside_pipe(t_token *token)
{
	while (token && token->identifier != PIPE)
		token = token->next;
	token = token->prev;
	while (token && !is_operator(token->text[0]))
		token = token->prev;
	return (token);
}

size_t	count_nb_operators_inside_pipe(t_token *token)
{
	size_t	counter;

	counter = 0;
	while (token && token->identifier != PIPE)
	{
		if (is_operator(token->text[0]))
			++counter;
		token = token->prev;
	}
	return (counter);
}

t_token	*find_next_op_inside_pipe(t_token *token)
{
	t_token	*tmp;

	while (token && token->identifier != PIPE)
	{
		if (is_operator(token->text[0]))
			return (token);
		tmp = token;
		token = token->prev;
	}
	return (tmp);
}

bool	fill_left_inside(t_ast **ast, t_token *token)
{
	size_t	nb_operators_inside_pipe;

	nb_operators_inside_pipe = count_nb_operators_inside_pipe(token);
	while (nb_operators_inside_pipe)
	{
		token = find_next_op_inside_pipe(token->prev); 
		(*ast)->left = new_node(token);
		if ((*ast)->left == NULL)
			return (false);
		(*ast)->left->parent = malloc(sizeof(t_ast));
		if ((*ast)->left->parent == NULL)
			return (false);
		(*ast)->left->parent = *ast;
		if (token->identifier == CMD && token->next->identifier == ARGUMENT)
		{
			(*ast)->left->argv = insert_argv(token);
			if ((*ast)->left->argv == NULL) 
				return (false);
		}
		if (is_operator(token->text[0]))
		{
			(*ast)->left->right = new_node(token->next);
			if ((*ast)->left->right == NULL)
				return (false);
			(*ast)->left->right->parent = malloc(sizeof(t_ast));
			if ((*ast)->left->right->parent == NULL)
				return (false);
			(*ast)->left->right->parent = (*ast)->left;
		}
		// TODO: to change
		ast = &((*ast)->left);
		--nb_operators_inside_pipe;
	}
	return (true);
}

bool	fill_left_with_pipe_parent(t_ast **ast, t_token *last)
{
	size_t	nb_pipes;
	t_token	*token;

	nb_pipes = count_nb_pipes(last);
	while (nb_pipes)
	{
		last = find_next_pipe_or_cmd(last->prev);
		if (last->identifier == CMD)
		{
			// TODO:  si on arrive au dernier bloc sans pipe, on implement differemment;
			if (!has_operator_inside_pipe(last))
			{
				(*ast)->left = new_node(last);
				if ((*ast)->left == NULL)
					return (false);
				(*ast)->left->parent = malloc(sizeof(t_ast));
				if ((*ast)->left->parent == NULL)
					return (false);
				(*ast)->left->parent = *ast;
				if (last->next->identifier == ARGUMENT)
				{
					(*ast)->left->argv = insert_argv(last);
					if ((*ast)->left->argv == NULL) 
						return (false);
				}
			}
			else
			{
				last = find_operator_inside_pipe(last);
				(*ast)->left = new_node(last);
				if ((*ast)->left == NULL)
					return (false);
				(*ast)->left->parent = malloc(sizeof(t_ast));
				if ((*ast)->left->parent == NULL)
					return (false);
				(*ast)->left->parent = *ast;
				(*ast)->left->right = new_node(last->next);
				(*ast)->left->right->parent = malloc(sizeof(t_ast));
				if ((*ast)->left->right->parent == NULL)
					return (false);
				(*ast)->left->right->parent = (*ast)->left;
				if ((*ast)->left->right == NULL)
					return (false);
				if (!fill_left_inside(&(*ast)->left, last))
					return (false);
			}
		}
		else
		{
			(*ast)->left = new_node(last);
			if ((*ast)->left == NULL)
				return (false);
			(*ast)->left->parent = malloc(sizeof(t_ast));
			if ((*ast)->left->parent == NULL)
				return (false);
			(*ast)->left->parent = *ast;
			if (!has_operator_inside_pipe(last->next))
			{
				(*ast)->left->right = new_node(last->next);
				if ((*ast)->left->right == NULL)
					return (false);
				(*ast)->left->right->parent = malloc(sizeof(t_ast));
				if ((*ast)->left->right->parent == NULL)
					return (false);
				(*ast)->left->right->parent = (*ast)->left;
				if (last->next->next->identifier == ARGUMENT)
				{
					(*ast)->left->right->argv = insert_argv(last->next);
					if ((*ast)->left->right->argv == NULL)
						return (false);
				}
			}
			else
			{
				token = find_operator_inside_pipe(last->next);
				(*ast)->left->right = new_node(token);
				if ((*ast)->left->right == NULL)
					return (false);
				(*ast)->left->right->parent = malloc(sizeof(t_ast));
				if ((*ast)->left->right->parent == NULL)
					return (false);
				(*ast)->left->right->parent = (*ast)->left;
				(*ast)->left->right->right = new_node(token->next);
				if ((*ast)->left->right->right == NULL)
					return (false);
				(*ast)->left->right->right->parent = malloc(sizeof(t_ast));
				if ((*ast)->left->right->right->parent == NULL)
					return (false);
				(*ast)->left->right->right->parent = (*ast)->left->right;
				if (!fill_left_inside(&(*ast)->left->right, token))
					return (false);
			}
		}
		ast = &((*ast)->left);
		--nb_pipes;
	}
	return (true);
}

bool	fill_left_with_pipe(t_ast **ast, t_token *last)
{
	size_t	nb_operators;

	nb_operators = count_nb_operators_with_pipe(last);
	while (nb_operators != 0)
	{
		last = find_next_ast_node_with_pipe(last->prev);
		(*ast)->left = new_node(last);
		(*ast)->left->parent = malloc(sizeof(t_ast));
		if ((*ast)->left->parent == NULL)
			return (false);
		(*ast)->left->parent = *ast;
		if ((*ast)->left->identifier == CMD && last->next->identifier == ARGUMENT)
		{
			(*ast)->left->argv = insert_argv(last->next);
			if ((*ast)->left->argv == NULL)
				return (false);
		}
		else if (is_operator(last->text[0]) && !is_operator(last->next->text[0]))
		{
			(*ast)->left->right = new_node(last->next); 		
			(*ast)->left->right->parent = malloc(sizeof(t_ast));
			if ((*ast)->left->right->parent == NULL)
				return (false);
			(*ast)->left->right->parent = (*ast)->left;
		}
		ast = &((*ast)->left);
		--nb_operators;
	}
	return (true);
}

bool	create_ast_without_pipe(t_ast **ast, t_token *last)
{
	*ast = new_node(last);
	if (*ast == NULL)
		return (false);
	(*ast)->right = new_node(last->next);
	if ((*ast)->right == NULL)
		return (false);
	(*ast)->right->parent = malloc(sizeof(t_ast));
	if ((*ast)->right->parent == NULL)
		return (false);
	(*ast)->right->parent = *ast;
	if (!fill_left(ast, last))
		return (false);
	return (true);
}

t_token	*find_next_right_node(t_token *token)
{
	t_token	*tmp;

	while (token && token->identifier != PIPE)
	{
		tmp = token;
		token = token->next;
	}
	if (!token)
	{
		while (!is_operator(tmp->text[0]))
			tmp = tmp->prev;
		return (tmp);
	}
	token = token->prev;
	while (!is_operator(token->text[0]))
		token = token->prev;
	return (token);
}

bool	create_ast_with_pipe(t_ast **ast, t_token *last)
{
	t_token	*token;

	*ast = new_node(last); 
	if (*ast == NULL)
		return (false);
	// TODO: RIGHT
	if (!has_operator_with_pipe(last->next))
	{
		(*ast)->right = new_node(last->next);
		(*ast)->right->parent = malloc(sizeof(t_ast));
		if ((*ast)->right->parent == NULL)
			return (false);
		(*ast)->right->parent = *ast;
		if ((*ast)->right == NULL)
			return (false);
		if (last->next->next)
		{
			(*ast)->right->argv = insert_argv(last->next);
			if ((*ast)->right->argv == NULL)
				return (false);
		}
	}
	else
	{
		token = find_next_right_node(last->next);
		(*ast)->right = new_node(token);
		(*ast)->right->parent = malloc(sizeof(t_ast));
		if ((*ast)->right->parent == NULL)
			return (false);
		(*ast)->right->parent = *ast;
		if ((*ast)->right == NULL)
			return (false);
		(*ast)->right->right = new_node(token->next);
		if ((*ast)->right->right == NULL)
			return (false);
		(*ast)->right->right->parent = malloc(sizeof(t_ast));
		if ((*ast)->right->right->parent == NULL)
			return (false);
		(*ast)->right->right->parent = (*ast)->right;
		if (!fill_left_with_pipe(&(*ast)->right, token))
			return (false);
	}
	// TODO: LEFT
	if (!fill_left_with_pipe_parent(ast, last))
		return (false);
	return (true);
}

t_ast	*parse(t_token *token_root)
{
	t_ast	*ast;
	t_token	*last;
	
	ast = NULL;
	if (!has_operator(token_root))
	{
		ast = create_cmd(token_root);
		if (ast == NULL)
			return (NULL);
	}
	else
	{
		last = tokens_find_last(token_root);	
		if (!has_pipe(last))
		{
			while (!is_operator(last->text[0]))
				last = last->prev;
			if (!create_ast_without_pipe(&ast, last))
				return (NULL);
		}
		else
		{
			while (last->text[0] != '|')
				last = last->prev;
			if (!create_ast_with_pipe(&ast, last))
				return (NULL);
		}
	}
	return (ast);
}
