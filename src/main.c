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
bool	parseline(t_minishell *mshell, char *line);
bool	empty_line(char *line);
int	in_env(t_minishell *mshell, char *line, int size, char **str);
int	dollar_point_interrogation(t_minishell *mshell, char **str);
int	add_dollar(char *line, int *index, char **str, t_minishell *mshell);
int	add_char(char *c, char **str, t_minishell *mshell, int *index);
int	replace_dollar(char **line, t_minishell *mshell);
void	quoting_choice(bool *dq, bool *sq, int *index, char c);
int	ft_search(char *str, char c);
int	end_word(char *str, char *env);
int	exist_in_env(char *line, int *i, t_minishell *mshell);
char	*get_elem_env(t_lst *env, char *key);

int	ft_search(char *str, char c)
{
	int	i;

	i = -1;
	while (str[++i])
		if (str[i] == c)
			return (i);
	return (0);
}

int	end_word(char *str, char *env)
{
	int	i;

	i = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		++i;
	if (i == ft_search(env, '='))
		return (i);
	return (0);
}

/* return 1 si $VAR dans env sinon 0 */
int	exist_in_env(char *line, int *i, t_minishell *mshell)
{
	t_lst	*tmp;
	int		len;

	if (line[*i + 1] == '?' || line[*i + 1] == '$')
		return (2);
	tmp = mshell->env;
	len = len_list(tmp);
	while (len--)
	{
		if (ft_strncmp(tmp->str, &line[*i + 1], \
			end_word(&line[*i + 1], tmp->str)) == 0)
		{
			*i += ft_strlen(tmp->str) - \
				ft_strlen(ft_strchr(tmp->str, '=')) + 1;
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

char	*get_elem_env(t_lst *env, char *key)
{
	t_lst	*tmp;
	int		len;
	int		t;

	if (!key)
		return (NULL);
	tmp = env;
	len = len_list(tmp);
	t = ft_strlen(key);
	while (len--)
	{
		if (ft_strncmp(tmp->str, key, t) == 0)
		{
			len = 0;
			while (tmp->str[len])
				if (tmp->str[len++] == '=')
					break ;
			return (ft_strdup(&(tmp->str[len])));
		}
		tmp = tmp->next;
	}
	return (NULL);
}

char	*get_dollar_word(char *line, int size)
{
	char	*dollar;
	int		i;

	dollar = malloc(sizeof(char) * size);
	if (!dollar)
		return (NULL);
	i = 0;
	while (line[++i] && i < size)
		dollar[i - 1] = line[i];
	dollar[i - 1] = '\0';
	return (dollar);
}

void	quoting_choice(bool *dq, bool *sq, int *index, char c)
{
	if ((c == '\'' || c == '"') && !*sq && !*dq)
	{
		if (c == '\'' && !*dq)
			*sq = true;
		else if (c == '"' && !*sq)
			*dq = true;
		if (index)
			++(*index);
	}
	else if ((c == '\'' || c == '"'))
	{
		if (c == '\'' && !*dq && *sq)
			*sq = false;
		else if (c == '"' && !*sq && *dq)
			*dq = false;
		if (index)
			++(*index);
	}
}

int	in_env(t_minishell *mshell, char *line, int size, char **str)
{
	char	*tmp;
	char	*key;
	char	*value;

	key = get_dollar_word(line, size);
	value = get_elem_env(mshell->env, key);
	if (key)
		free(key);
	tmp = ft_strjoin(*str, value);
	if (value)
		free(value);
	free(*str);
	if (!tmp)
		return (0);
	*str = tmp;
	return (1);
}

int	dollar_point_interrogation(t_minishell *mshell, char **str)
{
	char	*tmp;
	char	*tmp2;

	tmp = ft_itoa(mshell->exit_code);
	if (!tmp)
		return (0);
	tmp2 = ft_strjoin(*str, tmp);
	free(tmp);
	free(*str);
	if (!tmp2)
		return (0);
	*str = tmp2;
	return (1);
}

int	add_dollar(char *line, int *index, char **str, t_minishell *mshell)
{
	int		ctrl;
	int		n;

	n = *index;
	ctrl = exist_in_env(line, index, mshell);
	if (ctrl == 1)
		return (in_env(mshell, &line[n], *index - n, str));
	else if (ctrl == 2)
	{
		(*index) += 2;
		return (dollar_point_interrogation(mshell, str));
	}
	else
	{
		++(*index);
		while (line[*index] && \
			(ft_isalnum(line[*index]) || line[*index] == '_'))
			++(*index);
		return (1);
	}
}

int	add_char(char *c, char **str, t_minishell *mshell, int *index)
{
	char	char_to_str[2];
	char	*tmp2;
	int		i;

	i = 0;
	if (c[i] == '$' && !mshell->sq && exist_in_env(c, &i, mshell))
		return (1);
	char_to_str[0] = *c;
	char_to_str[1] = '\0';
	(*index)++;
	tmp2 = ft_strjoin(*str, char_to_str);
	free(*str);
	if (!tmp2)
		return (0);
	*str = tmp2;
	return (1);
}

int	replace_dollar(char **line, t_minishell *mshell)
{
	bool	dq;
	int		i;
	char	*str;

	i = 0;
	dq = false;
	mshell->sq = false;
	str = ft_strdup("");
	while ((*line)[i])
	{
		quoting_choice(&dq, &mshell->sq, NULL, (*line)[i]);
		if ((*line)[i] && (*line)[i + 1] && (*line)[i] == '$' && \
			((*line)[i + 1] != '\'' && (*line)[i + 1] != '"') && \
			(ft_isalpha((*line)[i + 1]) || (*line)[i + 1] == '?' || \
			(*line)[i + 1] == '_') && !mshell->sq && \
			!add_dollar((*line), &i, &str, mshell))
			return (0);
		if ((*line)[i] && !add_char(&(*line)[i], &str, mshell, &i))
			return (0);
	}
	free(*line);
	*line = str;
	return (1);
}

bool	parseline(t_minishell *mshell, char *line)
{
	printf("line: %s\n", line);
	replace_dollar(&line, mshell);
	printf("line: %s\n", line);
	exit(69);
	/*print_token(mshell->token);*/
	/*if (mshell->token && mshell->token->prev->type == PIPE)*/
	/*{*/
		/*write(2, "Error: Unclosed pipe\n", 21);*/
		/*mshell->exit_code = 2;*/
		/*free_token(&mshell->token);*/
		/*return (false);*/
	/*}*/
	/*if (!mshell->token || !create_list_cmd(mshell))*/
	/*{*/
		/*free_token(&mshell->token);*/
		/*free_cmd(&mshell->cmd);*/
		/*return (false);*/
	/*}*/
	/*return (check_pipe(mshell));*/
	return (true);
}

bool	empty_line(char *line)
{
	int	i;

	i = 0;
	while (line[i] && is_space(line[i]))
		i++;
	if (i == (int)ft_strlen(line))
	{
		free(line);
		return (true);
	}
	return (false);
}

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
	append(&(mshell->env), tmp);
	tmp = ft_strjoin("PWD=", path);
	append(&(mshell->env), tmp);
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
	listen_signals();
	while (1)
	{
		entry = readline("minishell> ");
		if (entry == NULL)
		{
			printf("exit\n");
			exit(1);
		}
		if (has_open_quote(entry, false, 0))
			printf("open quote\n");
		else
		{
			if (empty_line(entry))
				continue ;
			add_history(entry);
			if (!parseline(&mshell, entry))
				continue ;
		}
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
	if (!repl(env, path))
		return (1);
	return (0);
}
