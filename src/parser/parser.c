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
	ast->argv = malloc((list_size(token_root) + 2) * sizeof(char *));
	if (ast->argv == NULL)
		return (NULL);
	ast->argv[0] = ft_strdup(ast->text);
	if (ast->argv[0] == NULL)
		return (NULL);
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

// TODO
//t_ast *create_cmd(t_token *t)
//{
//}
// echo "$HOME donto" man negga 
t_ast	*parse(t_token *token_root)
{
	t_ast	*ast;
	
	ast = NULL;
	if (!has_operator(token_root))
	{
		ast = create_cmd(token_root);
		if (ast == NULL)
			return (NULL);
	}
	return (ast);
}
