/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmihangy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 12:13:57 by pmihangy          #+#    #+#             */
/*   Updated: 2024/11/11 09:22:58 by pmihangy         ###   ########.fr       */
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

typedef enum	e_identifier
{
	CMD,
	ARGUMENT,
	LEFT_REDIRECTION,
	RIGHT_REDIRECTION,
	DOUBLE_LEFT_REDIRECTION,
	DOUBLE_RIGHT_REDIRECTION,
	PIPE
}	t_identifier;

typedef struct	s_token
{
	char			*text;
	t_identifier	identifier;
	struct s_token	*prev;
	struct s_token	*next;
}	t_token;

typedef struct	s_ast
{
	char			*text;
	t_identifier	identifier;
	char			**argv;
	struct s_ast	*left;
	struct s_ast	*right;
	struct s_ast	*parent;
}	t_ast;

typedef struct	s_verif
{
	bool	res;
	char	*name;
}	t_verif;

/* prototypes */

/* error */
bool	has_open_quote(char *entry, bool found_peer, int i);

/* utils */
bool	is_space(char c);
void	print_tokens(t_token *root);
void	print_ast(t_ast *ast, int indent);
t_verif	*validate(t_token *token_root, char *path, char **builtins);
char	*get_path(char **env);
bool	insert_builtins(char **builtins);
bool	is_in(char *needle, char **haystack);
char	*get_dir_path(char *cmd, char *path);

/* builtins */
void	echo_minishell(char **argv);
void	env_minishell(char **env);
bool	export_minishell(char ***env, char **to_export);
bool	unset_minishell(char ***env, char **to_unset);	
void	pwd_minishell(void);
int		cd_minishell(char *argv[], char ***env);
void	exit_minishell(char **argv);

#endif
