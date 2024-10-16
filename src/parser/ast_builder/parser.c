/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmihangy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 09:15:59 by pmihangy          #+#    #+#             */
/*   Updated: 2024/10/07 09:52:17 by pmihangy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

bool	has_operator(t_token *token)
{
	while (token)
	{
		IF_RETURN(is_operator(token->text[0]), true)
		token = token->next;
	}
	return (false);
}

bool	has_operator_with_pipe(t_token *token)
{
	while (token && token->identifier != PIPE)
	{
		IF_RETURN(is_operator(token->text[0]), true)
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
	IF_RETURN(!ast, NULL)
	ast->text = token_root->text;
	ast->identifier = token_root->identifier;
	ast->argv = NULL;
	ast->left = NULL;
	ast->right = NULL;
	ast->parent = NULL;
	token_root = token_root->next;
	if (token_root)
	{
		ast->argv = malloc((list_size(token_root) + 1) * sizeof(char *));
		IF_RETURN(!ast->argv, NULL)
	}
	i = 0;
	while (token_root)
	{
		ast->argv[i] = ft_strdup(token_root->text);
		IF_RETURN(!ast->argv[i], NULL)
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
		IF_RETURN(t->text[0] == '|', true)
		t = t->prev;
	}
	return (false);
}

t_ast	*new_node(t_token *token)
{
	t_ast	*new;

	new = malloc(sizeof(t_ast));
	IF_RETURN(!new, NULL)
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
	IF_RETURN(!last, tmp)
	return (last);
}

t_token	*find_next_ast_node_with_pipe(t_token *last)
{
	t_token	*tmp;

	while (last && last->identifier != PIPE && !is_operator(last->text[0]))
	{
		tmp = last;
		last = last->prev;
	}
	IF_RETURN(last->identifier == PIPE || !last, tmp)
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
	IF_RETURN(!token, tmp)
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
	IF_RETURN(!argv, NULL)
	curr = last;
	i = 0;
	while (curr && !is_operator(curr->text[0]))
	{
		argv[i] = ft_strdup(curr->text);
		IF_RETURN(!argv[i], NULL)
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
		IF_RETURN((*ast)->left == NULL, false)
		(*ast)->left->parent = malloc(sizeof(t_ast));
		IF_RETURN((*ast)->left->parent == NULL, false)
		(*ast)->left->parent = *ast;
		if ((*ast)->left->identifier == CMD && last->next->identifier == ARGUMENT)
		{
			(*ast)->left->argv = insert_argv(last->next);
			IF_RETURN((*ast)->left->argv == NULL, false)
		}
		else if (is_operator(last->text[0]) && !is_operator(last->next->text[0]))
		{
			(*ast)->left->right = new_node(last->next); 		
			IF_RETURN((*ast)->left->right == NULL, false)
			(*ast)->left->right->parent = malloc(sizeof(t_ast));
			IF_RETURN((*ast)->left->right->parent == NULL, false)
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
		IF_RETURN(is_operator(token->text[0]), true)
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
		IF_RETURN((*ast)->left == NULL, false)
		(*ast)->left->parent = malloc(sizeof(t_ast));
		IF_RETURN((*ast)->left->parent == NULL, false)
		(*ast)->left->parent = *ast;
		if (token->identifier == CMD && token->next->identifier == ARGUMENT)
		{
			(*ast)->left->argv = insert_argv(token);
			IF_RETURN((*ast)->left->argv == NULL, false)
		}
		if (is_operator(token->text[0]))
		{
			(*ast)->left->right = new_node(token->next);
			IF_RETURN((*ast)->left->right == NULL, false)
			(*ast)->left->right->parent = malloc(sizeof(t_ast));
			IF_RETURN((*ast)->left->right->parent == NULL, false)
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
				IF_RETURN((*ast)->left == NULL, false)
				(*ast)->left->parent = malloc(sizeof(t_ast));
				IF_RETURN((*ast)->left->parent == NULL, false)
				(*ast)->left->parent = *ast;
				if (last->next->identifier == ARGUMENT)
				{
					(*ast)->left->argv = insert_argv(last);
					IF_RETURN((*ast)->left->argv == NULL, false)
				}
			}
			else
			{
				last = find_operator_inside_pipe(last);
				(*ast)->left = new_node(last);
				IF_RETURN((*ast)->left == NULL, false)
				(*ast)->left->parent = malloc(sizeof(t_ast));
				IF_RETURN((*ast)->left->parent == NULL, false)
				(*ast)->left->parent = *ast;
				(*ast)->left->right = new_node(last->next);
				(*ast)->left->right->parent = malloc(sizeof(t_ast));
				IF_RETURN((*ast)->left->right->parent == NULL, false)
				(*ast)->left->right->parent = (*ast)->left;
				IF_RETURN((*ast)->left->right == NULL, false)
				IF_RETURN(!fill_left_inside(&(*ast)->left, last), false)
			}
		}
		else
		{
			(*ast)->left = new_node(last);
			IF_RETURN((*ast)->left == NULL, false)
			(*ast)->left->parent = malloc(sizeof(t_ast));
			IF_RETURN((*ast)->left->parent == NULL, false)
			(*ast)->left->parent = *ast;
			if (!has_operator_inside_pipe(last->next))
			{
				(*ast)->left->right = new_node(last->next);
				IF_RETURN((*ast)->left->right == NULL, false)
				(*ast)->left->right->parent = malloc(sizeof(t_ast));
				IF_RETURN((*ast)->left->right->parent == NULL, false)
				(*ast)->left->right->parent = (*ast)->left;
				if (last->next->next->identifier == ARGUMENT)
				{
					(*ast)->left->right->argv = insert_argv(last->next);
					IF_RETURN((*ast)->left->right->argv == NULL, false)
				}
			}
			else
			{
				token = find_operator_inside_pipe(last->next);
				(*ast)->left->right = new_node(token);
				IF_RETURN((*ast)->left->right == NULL, false)
				(*ast)->left->right->parent = malloc(sizeof(t_ast));
				IF_RETURN((*ast)->left->right->parent == NULL, false)
				(*ast)->left->right->parent = (*ast)->left;
				(*ast)->left->right->right = new_node(token->next);
				IF_RETURN((*ast)->left->right->right == NULL, false)
				(*ast)->left->right->right->parent = malloc(sizeof(t_ast));
				IF_RETURN((*ast)->left->right->right->parent == NULL, false)
				(*ast)->left->right->right->parent = (*ast)->left->right;
				IF_RETURN(!fill_left_inside(&(*ast)->left->right, token), false)
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
		IF_RETURN((*ast)->left == NULL, false)
		(*ast)->left->parent = malloc(sizeof(t_ast));
		IF_RETURN((*ast)->left->parent == NULL, false)
		(*ast)->left->parent = *ast;
		if ((*ast)->left->identifier == CMD && last->next->identifier == ARGUMENT)
		{
			(*ast)->left->argv = insert_argv(last->next);
			IF_RETURN((*ast)->left->argv == NULL, false)
		}
		else if (is_operator(last->text[0]) && !is_operator(last->next->text[0]))
		{
			(*ast)->left->right = new_node(last->next); 		
			IF_RETURN((*ast)->left->right, false)
			(*ast)->left->right->parent = malloc(sizeof(t_ast));
			IF_RETURN((*ast)->left->right->parent == NULL, false)
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
	IF_RETURN(*ast == NULL, false)
	(*ast)->right = new_node(last->next);
	IF_RETURN((*ast)->right == NULL, false)
	(*ast)->right->parent = malloc(sizeof(t_ast));
	IF_RETURN((*ast)->right->parent == NULL, false)
	(*ast)->right->parent = *ast;
	IF_RETURN(!fill_left(ast, last), false)
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
	IF_RETURN(*ast == NULL, false)
	// TODO: RIGHT
	if (!has_operator_with_pipe(last->next))
	{
		(*ast)->right = new_node(last->next);
		IF_RETURN((*ast)->right == NULL, false)
		(*ast)->right->parent = malloc(sizeof(t_ast));
		IF_RETURN((*ast)->right->parent == NULL, false)
		(*ast)->right->parent = *ast;
		if (last->next->next)
		{
			(*ast)->right->argv = insert_argv(last->next);
			IF_RETURN((*ast)->right->argv == NULL, false)
		}
	}
	else
	{
		token = find_next_right_node(last->next);
		(*ast)->right = new_node(token);
		IF_RETURN((*ast)->right == NULL, false)
		(*ast)->right->parent = malloc(sizeof(t_ast));
		IF_RETURN((*ast)->right->parent == NULL, false)
		(*ast)->right->parent = *ast;
		(*ast)->right->right = new_node(token->next);
		IF_RETURN((*ast)->right->right == NULL, false)
		(*ast)->right->right->parent = malloc(sizeof(t_ast));
		IF_RETURN((*ast)->right->right->parent == NULL, false)
		(*ast)->right->right->parent = (*ast)->right;
		IF_RETURN(!fill_left_with_pipe(&(*ast)->right, token), false)
	}
	// TODO: LEFT
	IF_RETURN(!fill_left_with_pipe_parent(ast, last), false)
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
		IF_RETURN(!ast, NULL)
	}
	else
	{
		last = tokens_find_last(token_root);	
		if (!has_pipe(last))
		{
			while (!is_operator(last->text[0]))
				last = last->prev;
			IF_RETURN(!create_ast_without_pipe(&ast, last), NULL)
		}
		else
		{
			while (last->text[0] != '|')
				last = last->prev;
			IF_RETURN(!create_ast_with_pipe(&ast, last), NULL)
		}
	}
	return (ast);
}
