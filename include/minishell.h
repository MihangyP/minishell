/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmihangy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 12:13:57 by pmihangy          #+#    #+#             */
/*   Updated: 2024/09/17 11:54:18 by pmihangy         ###   ########.fr       */
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
# include <readline/readline.h>
# include <readline/history.h>
# include <libft.h>
#include <signal.h>

typedef void (*sighandler_t)(int);


/* structures and enums */

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

// ex: echo "donto" > donto.txt | ls -l | pwd
/*
 *							|
 *					|  				pwd
 *			     >     ls -l
 *	echo "donto"   donto.txt
 *
 */

typedef struct	s_ast
{
	char			*text;
	t_identifier	identifier;
	char			**argv; // if CMD
	struct s_ast	*left;
	struct s_ast	*right;
}	t_ast;

/* prototypes */

/* error */
bool	has_open_quote(char *entry, bool found_peer, int i);

/* utils */
bool	is_space(char c);
void	print_tokens(t_token *root);

/* parser */

t_token	*lexer(char *entry);
t_ast	*parse(t_token *token_root);
/* bool */
bool	is_operator(char c);
bool	has_an_env(char *text);
/* list manip */
t_token	*new_token(char *text, t_identifier identifier);
t_token	*tokens_find_last(t_token *root);
size_t	list_size(t_token *token);
void	tokens_append(t_token **root, t_token *new_el);
/* utils */
int		trim_spaces(char *entry, int *cur);
int		calc_text_token_len(char *entry, int i);
int		calc_text_in_quote_len(char *entry, int i, char quote);
int		calc_env_len(char *text, int start);
int		count_text_size(char *text);

#endif
