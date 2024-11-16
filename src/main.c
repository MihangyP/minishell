/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmihangy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 12:13:11 by pmihangy          #+#    #+#             */
/*   Updated: 2024/11/16 08:33:54 by pmihangy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	list_new_elem_str(t_lst **new, char *elem);
void	print_lst(t_lst *list);
void	add_first(t_token **list, t_token *new);
size_t	len_list(t_lst *list);
int	append(t_lst **lst, char *elem);
int	free_list(t_lst **list);
bool	make_env2(t_minishell *mshell);
int	env_init(t_minishell *mshell, char **env);
void	error(const char *err_mess);
bool	is_space(char c);
char	*get_path(char **env);
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
bool	create_list_token(t_token **begin, char *command);
bool	add_cmd(t_token **begin, char **command);
bool	add_special(t_token **begin, char **command);
void	free_token(t_token **list);
int	length_cmd(char *command, int *quotes);
void	copy_token(char *command, int length, char *str, int i);
int	append_token(t_token **list, char *str, int type);
int	token_new_elem(t_token **new, char *str, int type);
int	is_special(char *str);
void	add_first_list(t_lst **list, t_lst *new);
int	token_new_elem(t_token **new, char *str, int type);
void	print_token(t_token *token);
bool	create_list_cmd(t_minishell *mshell);
bool	norm(t_minishell *mshell, t_token *tmp);
bool	fill_cmd(t_minishell *mshell, t_token *tmp);
int	cmd_new_elem(t_cmd **new, int infile, int outfile, char **cmd_param);
int	append_cmd(t_cmd **list, int infile, int outfile, char **cmd_param);
char	**get_param(t_minishell *mshell, t_token *token);

char	**get_param(t_minishell *mshell, t_token *token)
{
	char	**cmd_param;
	int		i;
	t_token	*tmp;

	i = 0;
	cmd_param = malloc(sizeof(char *) * (count_args(mshell, token) + 1));
	if (cmd_param == NULL)
		return (NULL);
	tmp = token;
	if (tmp->type != PIPE && (tmp->type == CMD || (tmp->type == ARG && \
		tmp->prev != mshell->token->prev && tmp->prev->type > 5)) && \
		!add_to_cmd_param(cmd_param, &i, tmp->str))
		return (free_cmd_param(cmd_param, i));
	tmp = tmp->next;
	while (tmp != mshell->token && tmp->type != PIPE)
	{
		if ((tmp->type == CMD || (tmp->type == ARG && \
			tmp->prev != mshell->token->prev && tmp->prev->type > 5)) && \
			!add_to_cmd_param(cmd_param, &i, tmp->str))
			return (free_cmd_param(cmd_param, i));
		tmp = tmp->next;
	}
	cmd_param[i] = NULL;
	return (cmd_param);
}

int	cmd_new_elem(t_cmd **new, int infile, int outfile, char **cmd_param)
{
	(*new) = malloc(sizeof(t_cmd));
	if (*new == NULL)
		return (0);
	(*new)->skip_cmd = false;
	(*new)->infile = infile;
	(*new)->outfile = outfile;
	(*new)->cmd_param = cmd_param;
	(*new)->next = NULL;
	(*new)->prev = NULL;
	return (1);
}

int	append_cmd(t_cmd **list, int infile, int outfile, char **cmd_param)
{
	t_cmd	*new;

	if (!cmd_new_elem(&new, infile, outfile, cmd_param))
		return (0);
	if (!(*list))
	{
		(*list) = new;
		(*list)->prev = *list;
		(*list)->next = *list;
	}
	else
	{
		new->prev = (*list)->prev;
		new->next = (*list);
		(*list)->prev->next = new;
		(*list)->prev = new;
	}
	return (1);
}

bool	fill_cmd(t_minishell *mshell, t_token *tmp)
{
	if (!get_infile(mshell, tmp, mshell->cmd->prev) && \
		mshell->cmd->prev->infile != -1)
		return (false);
	if (mshell->cmd->prev->infile == -1)
	{
		mshell->cmd->prev->skip_cmd = true;
		mshell->cmd->prev->outfile = -1;
		return (true);
	}
	if (!get_outfile(tmp, mshell->cmd->prev, mshell) && mshell->cmd->prev->outfile \
		!= -1)
		return (false);
	if (mshell->cmd->prev->outfile == -1)
	{
		if (mshell->cmd->prev->infile >= 0)
			close(mshell->cmd->prev->infile);
		mshell->cmd->prev->skip_cmd = true;
		mshell->cmd->prev->infile = -1;
		return (true);
	}
	mshell->cmd->prev->cmd_param = get_param(mshell, tmp);
	if (!mshell->cmd->prev->cmd_param)
		exit(69);
		/*free_all(mshell, ERR_MALLOC, EXT_MALLOC);*/
	return (true);
}

bool	norm(t_minishell *mshell, t_token *tmp)
{
	if (!append_cmd(&mshell->cmd, -2, -2, NULL))
		exit(69);
		/*free_all(mshell, ERR_MALLOC, EXT_MALLOC);*/
	if (!fill_cmd(mshell, tmp))
	{
		mshell->exit_code = 2;
		return (false);
	}
	return (true);
}

bool	create_list_cmd(t_minishell *mshell)
{
	t_token	*tmp;

	tmp = mshell->token;
	if (!norm(mshell, tmp))
		return (false);
	tmp = tmp->next;
	while (tmp != mshell->token)
	{
		if (tmp->prev->type == PIPE)
		{
			if (!norm(mshell, tmp))
				return (false);
		}
		tmp = tmp->next;
	}
	return (true);
}

void	print_token(t_token *token)
{
	t_token	*tmp;

	tmp = token;
	while (tmp->next != token)
	{
		printf("Type : %d, [%s]\n", tmp->id, tmp->text);
		tmp = tmp->next;
	}
	printf("Type : %d, [%s]\n",  tmp->id, tmp->text);
}

int	token_new_elem(t_token **new, char *str, int type)
{
	if (!str)
		return (0);
	(*new) = malloc(sizeof(t_token));
	if (*new == NULL)
	{
		free(str);
		return (0);
	}
	(*new)->text = str;
	(*new)->id = type;
	(*new)->next = NULL;
	(*new)->prev = NULL;
	return (1);
}

void	add_first_list(t_lst **list, t_lst *new)
{
	(*list) = new;
	(*list)->prev = *list;
	(*list)->next = *list;
}

int	append_token(t_token **list, char *str, int type)
{
	t_token	*new;

	if (!token_new_elem(&new, str, type))
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

void	copy_token(char *command, int length, char *str, int i)
{
	int	j;

	j = 0;
	while (command[i + j] && i < length)
	{
		if (command[i + j] == '\'' && ++j)
		{
			while (command[i + j] != '\'' && ++i)
				str[i - 1] = command[(i - 1) + j];
			j++;
		}
		else if (command[i + j] == '"' && ++j)
		{
			while (command[i + j] != '"' && ++i)
				str[i - 1] = command[(i - 1) + j];
			j++;
		}
		else
		{
			str[i] = command[i + j];
			i++;
		}
	}
	str[i] = 0;
}

int	length_cmd(char *command, int *quotes)
{
	int	i;

	i = 0;
	*quotes = 0;
	while (command[i] && !is_space(command[i]) && !is_special(command + i))
	{
		if (command[i] == '"' || command[i] == '\'')
		{
			(*quotes)++;
			if (command[i++] == '"')
				while (command[i] && command[i] != '"')
					++i;
			else
				while (command[i] && command[i] != '\'')
					++i;
			if (command[i])
				++i;
		}
		if (command[i] && command[i] != '"' && command[i] != '\'' && \
			!is_space(command[i]) && !is_special(command + i))
			++i;
	}
	return (i);
}

void	free_token(t_token **list)
{
	t_token	*tmp;
	t_token	*current;

	if (!(*list))
		return ;
	current = *list;
	while (current->next != *list)
	{
		tmp = current;
		current = current->next;
		free(tmp->text);
		free(tmp);
	}
	free(current->text);
	free(current);
	*list = NULL;
}

int	is_special(char *str)
{
	if (str && *str && ft_strlen(str) >= 2)
	{
		if (!ft_strncmp(str, "<<", 2))
			return (HEREDOC);
		if (!ft_strncmp(str, ">>", 2))
			return (APPEND);
	}
	if (*str && ft_strlen(str) >= 1)
	{
		if (!ft_strncmp(str, "<", 1))
			return (INPUT);
		if (!ft_strncmp(str, ">", 1))
			return (TRUNC);
		if (!ft_strncmp(str, "|", 1))
			return (PIPE);
	}
	return (0);
}

bool	add_cmd(t_token **begin, char **command)
{
	char	*str;
	int		length;
	int		quotes;
	int		i;

	i = 0;
	length = length_cmd(*command, &quotes);
	if (((length) - (2 * quotes)) < 0)
		return (true);
	str = malloc(sizeof(char) * ((length + 1) - (2 * quotes)));
	if (!str)
		return (false);
	copy_token(*command, length - (2 * quotes), str, i);
	if (!append_token(begin, str, 0))
		return (false);
	if ((*begin)->prev == (*begin) || (*begin)->prev->prev->id == PIPE)
		(*begin)->prev->id = CMD;
	else
		(*begin)->prev->id = ARG;
	(*command) += length;
	return (true);
}

bool	add_special(t_token **begin, char **command)
{
	int	spe;

	spe = is_special(*command);
	if (!spe)
		return (false);
	if (spe == INPUT && !append_token(begin, ft_strdup("<"), INPUT))
		return (false);
	else if (spe == HEREDOC && !append_token(begin, ft_strdup("<<"), HEREDOC))
		return (false);
	else if (spe == TRUNC && !append_token(begin, ft_strdup(">"), TRUNC))
		return (false);
	else if (spe == APPEND && !append_token(begin, ft_strdup(">>"), APPEND))
		return (false);
	else if (spe == PIPE && !append_token(begin, ft_strdup("|"), PIPE))
		return (false);
	if (spe == INPUT || spe == TRUNC || spe == PIPE)
		(*command)++;
	else if (spe == HEREDOC || spe == APPEND)
		(*command) += 2;
	return (true);
}

bool	create_list_token(t_token **begin, char *command)
{
	(*begin) = NULL;
	while (*command)
	{
		while (is_space(*command))
			command++;
		if (*command && !is_special(command) && !add_cmd(begin, &command))
		{
			if (*begin)
				free_token(begin);
			return (false);
		}
		else if (*command && is_special(command) && \
					!add_special(begin, &command))
		{
			if (*begin)
				free_token(begin);
			return (false);
		}
	}
	return (true);
}

int	ft_search(char *str, char c)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] == c)
			return (i);
	}
	return (0);
}

int	end_word(char *str, char *env)
{
	int	i;

	i = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		++i;
	/*if (i == ft_search(env, '='))*/
		/*return (i);*/
	return (ft_search(env, '='));
	/*return (0);*/
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
			((*line)[i + 1] != '\'' && (*line)[i + 1] != '\"') && \
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
	replace_dollar(&line, mshell);
	create_list_token(&mshell->token, line);
	print_token(mshell->token);
	if (mshell->token && mshell->token->prev->id == PIPE)
	{
		write(2, "Error: Unclosed pipe\n", 21);
		mshell->exit_code = 2;
		free_token(&mshell->token);
		return (false);
	}
	if (mshell->token)
		create_list_cmd(mshell);
	/*if (mshell->token)*/
		/*create_list_cmd(mshell); // TODO*/
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

void	add_first(t_token **list, t_token *new)
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
		add_first_list(list, new);
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
		else if (empty_line(entry))
				continue ;
		else
		{
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
