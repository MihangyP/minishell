#include <stdio.h>
#include <stdlib.h>

typedef enum
{
	COMMAND,
	ARGUMENT,
	REDIRECTION
}   e_type;

typedef struct node_ast
{
	e_type		type;
	char		*value;
	struct node_ast	*left;
	struct node_ast	*right;
}   t_node_ast;

t_node_ast	*create_ast(e_type type, char *value)
{
	t_node_ast	*node;

	node = (t_node_ast *)malloc(sizeof(t_node_ast));
	if (!node)
		return (NULL);
	node->type = type;
	node->value = value;
	node->right = NULL;
	node->left = NULL;
	return (node);
}

void	print_ast(t_node_ast *node, int indent)
{
	int	i;

	i = 0;
	if (!node)
		return ;
	while (i < indent)
	{
		printf("	");
		i++;
	}
	if (node->type == COMMAND)
		printf("commande : %s\n", node->value);
	if (node->type == ARGUMENT)
		printf("argument : %s\n", node->value);
	if (node->type == REDIRECTION)
		printf("redirection: %s\n", node->value);
	print_ast(node->left, indent + 1);
	print_ast(node->right, indent + 1);

}
void free_ast(t_node_ast *node)
{
	if (!node)
		return ;
	free(node->value);
	free_ast(node->left);
	free_ast(node->right);
	free(node);
}

int main()
{
	t_node_ast *ast_comm;
	t_node_ast *ast_arg;
	t_node_ast *ast_redirect;

	printf ("AST de : echo \"Hello\" > output.txt\n\n");
	ast_comm = create_ast(COMMAND, "echo");
	ast_arg = create_ast(ARGUMENT, "Hello");
	ast_redirect = create_ast(REDIRECTION, "outpout.txt");
	ast_comm->left = ast_arg;
	ast_comm->right = ast_redirect;
	
	print_ast(ast_comm, 0);
	return (0);
}
