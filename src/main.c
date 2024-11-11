/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmihangy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 12:13:11 by pmihangy          #+#    #+#             */
/*   Updated: 2024/11/11 10:21:26 by pmihangy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	error(const char *err_mess)
{
	printf("ERROR: %s\n", err_mess);
	exit(1);
}

bool	is_space(char c)
{
	return ((c >= 8 && c <= 13) || c == 32);
}

char	*get_path(char **env)
{
	int	i;

	i = 0;
	while (ft_strncmp(env[i], "PATH", 4))
		++i;
	return (env[i] + 5);
}

bool	is_operator(char c)
{
	return (c == '<' || c == '>' || c == '|');
}

bool	is_quote(char c)
{
	return (c == '\'' || c == '\"');
}

int	len_text(char *str)
{
	int	len;

	len = 0;
	while (str[len] && !is_operator(str[len]) && !is_space(str[len]) && !is_quote(str[len]))
		++len;
	return (len);
}

//$HOME /
//echo $HOME "hello $USER" 'ddfjf $PATH'
// TODO: verify if the text is in env
//^
int	line_expanded_len(char *str)
{
	int	len;
	int	i;
	char	quote;
	char	*text;

	len = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '\"' || str[i] == '\'')
		{
			quote = str[i];
			++i;
			while (str[i] != quote)
			{
				if (quote == '\'' || str[i] != '$')
				{
					++len;
					++i;
				}
				else
				{
					text = ft_substr(str, i + 1, len_text(str + i + 1));
					if (!text)
						error("Cannot substract the string\n");
					text = getenv(text);
					len += ft_strlen(text);
					i += len_text(str + i);
				}
			}
			len += 2;
			++i;
		}
		else
		{
			if (str[i] != '$')
			{
				++len;
				++i;
			}
			else
			{
				text = ft_substr(str, i + 1, len_text(str + i + 1));
				if (!text)
					error("Cannot substract the string\n");
				text = getenv(text);
				len += ft_strlen(text);
				i += len_text(str + i);
			}
		}
	}
	return (len);
}

void	expand(char **line)
{
	char	*str;
	char	*tmp;
	char	*text;
	int		i;
	int		j;
	int		str_size;
	char	quote;

	tmp = *line;
	str_size = line_expanded_len(tmp);
	str = malloc((str_size + 1) * sizeof(char));
	if (!str)
		error("malloc error");
	i = 0;
	j = 0;
	while (tmp[i])
	{
		if (tmp[i] == '\"' || tmp[i] == '\'')
		{
			quote = tmp[i];
			++i;
			str[j] = quote;
			++j;
			while (tmp[i] != quote)
			{
				if (quote == '\'' || tmp[i] != '$')
				{
					str[j] = tmp[i];
					++j;
					++i;
				}
				else
				{
					text = ft_substr(tmp, i + 1, len_text(tmp + i + 1));
					if (!text)
						error("Cannot substract the string\n");
					text = getenv(text);
					ft_memcpy(str + j, text, ft_strlen(text));
					j += ft_strlen(text);
					i += len_text(tmp + i);
				}
			}
			str[j] = quote;
			++j;
			++i;
		}
		else
		{
			if (tmp[i] != '$')
			{
				str[j] = tmp[i];
				++j;
				++i;
			}
			else
			{
				text = ft_substr(tmp, i + 1, len_text(tmp + i + 1));
				if (!text)
					error("Cannot substract the string\n");
				text = getenv(text);
				ft_memcpy(str + j, text, ft_strlen(text));
				j += ft_strlen(text);
				i += len_text(tmp + i);
			}
		}
	}
	str[j] = '\0';
	*line = str;
}

/*bool	lexer(t_token **token_root, char *line, char **env)*/
bool	lexer(char *line, char **env)
{
	expand(&line);
	printf("line: %s\n", line);
	/*tokenize(token_root, line);*/
	// expander && lexer
	/*if (!replace_dollar(&line, data) || !create_list_token(&data->token, line))*/
	/*{*/
		/*free(line);*/
		/*free_all(data, ERR_MALLOC, EXT_MALLOC);*/
	/*}*/
	/*free(line);*/
	/*print_token(data->token);*/
	/*// node3<-node1 -> node2 -> node3->node1*/
	/*if (data->token && data->token->prev->type == PIPE)*/
	/*{*/
		/*write(2, "Error: Unclosed pipe\n", 21);*/
		/*data->exit_code = 2;*/
		/*free_token(&data->token);*/
		/*return (false);*/
	/*}*/
	/*if (!data->token || !create_list_cmd(data))*/
	/*{*/
		/*free_token(&data->token);*/
		/*free_cmd(&data->cmd);*/
		/*return (false);*/
	/*}*/
	/*return (check_pipe(data));*/
	return (true);
}

void	handle_sigint(int signum)
{
	(void)signum;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	listen_signals(void)
{
	signal(SIGINT, handle_sigint);	
	signal(SIGQUIT, SIG_IGN);
}

bool	repl(char ***env, char *path)
{
	char		*entry;

	(void)path;
	(void)env;
	listen_signals();
	while (1)
	{
		entry = readline("minishell> ");
		if (entry == NULL)
		{
			printf("exit\n");
			exit(1);
		}
		add_history(entry);
		if (has_open_quote(entry, false, 0))
			printf("open quote\n");
		else
		{
			/*lexer(token_root, entry, *env);	*/
			lexer(entry, *env);	
		}
		free(entry);
	}
	rl_clear_history();
	return (true);
}

int	main(int ac, char **av, char **env)
{
	char	*path;

	(void)ac;
	(void)av;
	path = get_path(env);
	if (!repl(&env, path))
		return (1);
	return (0);
}
