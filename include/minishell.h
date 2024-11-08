/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmihangy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 12:13:57 by pmihangy          #+#    #+#             */
/*   Updated: 2024/11/07 17:05:08 by pmihangy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* libs */
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdbool.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <dirent.h>
# include <errno.h>
# include <libft.h>

/* structures and enums */

typedef void (*sighandler_t)(int);

typedef enum	e_id
{
	CMD,
	ARGUMENT,
	LEFT_REDIRECTION,
	RIGHT_REDIRECTION,
	DOUBLE_LEFT_REDIRECTION,
	DOUBLE_RIGHT_REDIRECTION,
	PIPE
}	t_id;

typedef struct	s_token
{
	char			*text;
	t_id			id;
	struct s_token	*prev;
	struct s_token	*next;
}	t_token;

typedef struct	s_ast
{
	char			*text;
	t_id			id;
	char			**argv;
	struct s_ast	*left;
	struct s_ast	*right;
	struct s_ast	*parent;
}	t_ast;

/* prototypes */

/* error */
bool	has_open_quote(char *entry, bool found_peer, int i);

/* utils */
bool	is_space(char c);
void	print_tokens(t_token *root);
void	print_ast(t_ast *ast, int indent);
void	validate(t_token *token_root, char *path);
char	*get_path(char **env);
bool	insert_builtins(char **builtins);
bool	is_in(char *needle, char **haystack);
char	*get_dir_path(char *cmd, char *path);
void	error(const char *err_message);

/* parser */

void	lexer(t_token **token_root, char *entry);
t_ast	*parse(t_token *token_root);
bool	insert_argument_token(char *entry, t_token **root, int *i);
bool	insert_cmd_token(char *entry, t_token **root, int *i);
/* bool */
bool	is_operator(char c);
bool	has_an_env(char *text);
/* list manip */
t_token	*new_token(char *text, t_id identifier);
t_token	*tokens_find_last(t_token *root);
size_t	list_size(t_token *token);
void	tokens_append(t_token **root, t_token *new_el);
/* utils */
int		trim_spaces(char *entry, int *cur);
int		calc_text_token_len(char *entry, int i);
int		calc_text_in_quote_len(char *entry, int i, char quote);
int		calc_env_len(char *text, int start);
int		count_text_size(char *text);
char	*get_variable_name(char *str);
bool	env_update(char ***env, char *str);
bool	verif_inside(char *directory, char *cmd);

/* builtins */
void	echo_minishell(char **argv);
void	env_minishell(char **env);
bool	export_minishell(char ***env, char **to_export);
bool	unset_minishell(char ***env, char **to_unset);	
void	pwd_minishell(void);
int		cd_minishell(char *argv[], char ***env);
void	exit_minishell(char **argv);

#endif
