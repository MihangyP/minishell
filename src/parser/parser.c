#include <minishell.h>

// ex: echo "donto" > donto.txt | ls -l | pwd
/*
 *							|
 *					|  				pwd
 *			     >     ls -l
 *	echo "donto"   donto.txt
 *
 */

/*typedef struct	s_ast*/
/*{*/
	/*char			*text;*/
	/*t_identifier	identifier;*/
	/*char			**argv; // if CMD*/
	/*struct s_ast	*left;*/
	/*struct s_ast	*right;*/
/*}	t_ast;*/

size_t	count_tokens(t_token *token)
{
	size_t	counter;

	counter = 0;
	while (token)
	{
		++counter;
		token = token->next;
	}
	return (counter);
}

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

bool	has_pipe(t_token *token)
{
	while (token)
	{
		if (token->identifier == PIPE)
			return (true);
		token = token->next;
	}
	return (false);
}

t_ast	*new_ast_node(char *text, t_identifier id)
{
	t_ast	*new;

	new = malloc(sizeof(t_ast));
	if (new == NULL)
		return (NULL);
	new->text = text;
	new->identifier = id;
	new->argv = NULL;
	new->left = NULL;
	new->right = NULL;
	return (new);
}

bool	build_an_ast_without_pipe(t_token *token, t_ast **ast)
{
	/*t_token	*last;*/
	int	i;

	if (!has_operator(token))
	{
		// TODO
		*ast = new_ast_node(token->text, token->identifier);
		if (*ast == NULL)
			return (false);
		token = token->next;
		(*ast)->argv = malloc((count_tokens(token) + 1) * sizeof(char *));
		if ((*ast)->argv == NULL)
			return (false);
		i = 0;
		while (token)
		{
			(*ast)->argv[i] = ft_strdup(token->text);
			++i;
			token = token->next;
		}
		(*ast)->argv[i] = NULL;
		return (true);
	}
	// TODO
	/*last = tokens_find_last(token); */
	/*while (token)*/
	/*{*/

		/*token = token->next;*/
	/*}*/
	return (true);
}

// TODO
t_ast	*parse(t_token *token_root)
{
	t_token	*last;
	t_ast	*ast;
	
	if (!has_pipe(token_root))
	{
		if (!build_an_ast_without_pipe(token_root, &ast))
			return (NULL);
		return (ast);
	}
	/*last = tokens_find_last(token_root);*/
	/*while (last)*/
	/*{*/
		/*last = last->prev;*/
	/*}*/
	return (NULL);
}
