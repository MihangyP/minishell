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

typedef enum	e_id
{
	CMD = 0,
	ARG,
	INPUT,
	TRUNC,
	HEREDOC,
	APPEND,
	PIPE
}	t_id;

typedef struct s_lst
{
	char			*str;
	struct s_lst	*prev;
	struct s_lst	*next;
}					t_lst;

typedef struct s_token
{
	char			*text;
	int				id;
	struct s_token	*prev;
	struct s_token	*next;
}				t_token;

typedef struct s_cmd
{
	bool			skip_cmd;
	int				infile;
	int				outfile;
	char			**cmd_param;
	struct s_cmd	*prev;
	struct s_cmd	*next;
}				t_cmd;

typedef struct s_minishell
{
	t_lst	*env;
	t_token	*token;
	t_cmd	*cmd;
	int		exit_code;
	int		pipefd[2];
	bool	sq;
}				t_minishell;

/* prototypes */

/* error */
bool	has_open_quote(char *entry, bool found_peer, int i);

/* utils */
bool	is_space(char c);
char	*get_path(char **env);
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
