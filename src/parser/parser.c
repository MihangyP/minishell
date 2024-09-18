#include <minishell.h>

/*typedef struct	s_ast*/
/*{*/
	/*char			*text;*/
	/*t_identifier	identifier;*/
	/*char			**argv; // if CMD*/
	/*struct s_ast	*left;*/
	/*struct s_ast	*right;*/
/*}	t_ast;*/

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

/*void	print_ast(t_ast *ast, int indent)*/
/*{*/
	/*int	i;*/

	/*i = 0;*/
	/*if (!ast)*/
		/*return ;*/
	/*while (i < indent)*/
	/*{*/
		/*printf("	");*/
		/*i++;*/
	/*}*/
	/*printf("%s\n", ast->text);*/
	/*print_ast(ast->left, indent + 1);*/
	/*print_ast(ast->right, indent + 1);*/
/*}*/

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

char	**insert_argv(t_token *last)
{
	char	**argv;
	int		size;
	int		i;
	t_token	*curr;

	size = 0;
	curr = last;
	while (!is_operator(curr->text[0]))
	{
		++size;
		curr = curr->next;
	}
	argv = malloc((size + 1) * sizeof(char *));
	if (argv == NULL)
		return (NULL);
	curr = last;
	i = 0;
	while (!is_operator(curr->text[0]))
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

// TODO
//t_ast *create_cmd(t_token *t)
//{
//}
// echo "$HOME donto" man negga 
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
			ast = new_node(last);
			if (ast == NULL)
				return (NULL);
			ast->right = new_node(last->next);
			if (ast->right == NULL)
				return (NULL);
			last = find_next_ast_node(last->prev);
			ast->left = new_node(last);
			if (ast->left->identifier == CMD && last->next->identifier == ARGUMENT)
			{
				ast->left->argv = insert_argv(last->next);
				if (ast->left->argv == NULL)
					return (NULL);
			}
		}
		/*else*/
		/*{*/
			/*while (last->text[0] != '|')*/
				/*last = last->prev;*/
		/*}*/
	}
	return (ast);
}
