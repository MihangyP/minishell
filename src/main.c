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

int	list_new_elem_str(t_lst **new, char *elem);
void	add_first(t_lst **list, t_lst *new);
size_t	len_list(t_lst *list);
int	append(t_lst **lst, char *elem);
int	free_list(t_lst **list);
bool	make_env2(t_minishell *mshell);
int	env_init(t_minishell *mshell, char **env);
void	error(const char *err_mess);
bool	is_space(char c);
char	*get_path(char **env);
bool	is_operator(char c);

void	print_lst(t_lst *list)
{
	t_lst	*curr;

	curr = list;
	while (curr->next != list)
	{
		printf("%s\n", curr->str);
		curr = curr->next;
	}
	printf("%s\n", curr->str);
}

int	list_new_elem_str(t_lst **new, char *elem)
{
	(*new) = malloc(sizeof(t_lst));
	if (*new == NULL)
		return (0);
	(*new)->str = elem;
	(*new)->next = NULL;
	(*new)->prev = NULL;
	return (1);
}

void	add_first(t_lst **list, t_lst *new)
{
	(*list) = new;
	(*list)->prev = *list;
	(*list)->next = *list;
}

size_t	len_list(t_lst *list)
{
	t_lst	*tmp;
	size_t	i;

	if ((list))
	{
		tmp = list;
		i = 1;
		while (tmp->next != list)
		{
			++i;
			tmp = tmp->next;
		}
		return (i);
	}
	return (0);
}

int	append(t_lst **list, char *elem)
{
	t_lst	*new;

	if (!list_new_elem_str(&new, elem))
		return (0);
	if (!(*list))
		add_first(list, new);
	else
	{
		new->prev = (*list)->prev;
		new->next = (*list);
		(*list)->prev->next = new;
		(*list)->prev = new;
	}
	return (1);
}

int	free_list(t_lst **list)
{
	t_lst	*tmp;
	t_lst	*current;

	if (!(*list))
		return (0);
	current = *list;
	while (current->next != *list)
	{
		tmp = current;
		current = current->next;
		free(tmp->str);
		free(tmp);
	}
	free(current->str);
	free(current);
	*list = NULL;
	return (0);
}

bool	make_env2(t_minishell *mshell)
{
	char	path[PATH_MAX];
	char	*tmp;

	tmp = ft_strdup("OLDPWD");
	if (!tmp || !append(&(mshell->env), tmp) || getcwd(path, PATH_MAX) == NULL)
		exit(69);
		/*free_all(mshell, ERR_MALLOC, EXT_MALLOC);*/
	tmp = ft_strjoin("PWD=", path);
	if (!tmp || !append(&(mshell->env), tmp))
		exit(69);
		/*free_all(mshell, ERR_MALLOC, EXT_MALLOC);*/
	return (1);
}

int	env_init(t_minishell *mshell, char **env)
{
	t_lst	*list;
	int		i;
	char	*tmp;

	if (!(*env))
		return (make_env2(mshell));
	i = -1;
	list = NULL;
	while (env[++i])
	{
		tmp = ft_strdup(env[i]);
		if (!tmp)
			return (free_list(&list));
		if (!append(&list, tmp))
			return (free_list(&list));
	}
	mshell->env = list;
	return (1);
}

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

void	minishell_init(t_minishell *mshell)
{
	mshell->env = NULL;
	mshell->token = NULL;
	mshell->cmd = NULL;
	mshell->exit_code = 0;
	mshell->pipefd[0] = -1;
	mshell->pipefd[1] = -1;
}

bool	repl(char **env, char *path)
{
	char		*entry;
	t_minishell	mshell;

	(void)path;
	minishell_init(&mshell);
	env_init(&mshell, env);
	print_lst(mshell.env);
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
			/*lexer(entry, *env);	*/
		}
		free(entry);
	}
	rl_clear_history();
	return (true);
}

/*int	main2(int argc, char **argv, char **env)*/
/*{*/
	/*t_data	data;*/
	/*char	*line;*/

	/*init_data(&data, argc, argv);*/
	/*if (!make_env(&data, env))*/
		/*free_all(&data, ERR_MALLOC, EXT_MALLOC);*/
	/*while (1)*/
	/*{*/
		/*line = readline("minishell> ");*/
		/*if (!line)*/
			/*free_all(&data, "exit\n", data.exit_code);*/
		/*if (empty_line(line))*/
			/*continue ;*/
		/*add_history(line);*/
		/*if (!parseline(&data, line))*/
			/*continue ;*/
		/*if (!exec(&data))*/
			/*free_all(&data, ERR_PIPE, EXT_PIPE);*/
		/*free_cmd(&data.cmd);*/
		/*free_token(&data.token);*/
	/*}*/
	/*rl_clear_history();*/
	/*free_all(&data, NULL, -1);*/
	/*return (0);*/
/*}*/

int	main(int ac, char **av, char **env)
{
	char	*path;

	(void)ac;
	(void)av;
	path = get_path(env);
	if (!repl(env, path))
		return (1);
	return (0);
}
