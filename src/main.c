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

// Global variable
pid_t	g_signal_pid;

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
bool	fill_cmd(t_minishell *mshell, t_token *tmp);
bool	norm(t_minishell *mshell, t_token *tmp);
bool	create_list_cmd(t_minishell *mshell);
int	cmd_new_elem(t_cmd **new, int infile, int outfile, char **cmd_param);
int	append_cmd(t_cmd **list, int infile, int outfile, char **cmd_param);
int	count_args(t_minishell *mshell, t_token *token);
int	add_to_cmd_param(char **cmd_param, int *i, char *str);
char	**get_param(t_minishell *mshell, t_token *token);
int	open_file(t_minishell *mshell, char *filename, int type);
bool	get_in(t_minishell *mshell, t_token *tmp, t_cmd *cmd);
bool	get_infile(t_minishell *mshell, t_token *token, t_cmd *cmd);
bool	get_out(t_token *tmp, t_cmd *cmd, t_minishell *mshell);
bool	get_outfile(t_token *token, t_cmd *cmd, t_minishell *mshell);
bool	print_error_token(t_token *token, t_minishell *mshell);
bool	read_in_stdin(t_minishell *mshell, int fd, char *word);
int	here_doc(t_minishell *mshell, char *word);
bool	check_pipe(t_minishell *mshell);
bool	is_builtin(char *cmd);
void	parent_process(t_minishell *mshell, t_cmd *cmd, int *pip);
bool	exec_cmd(t_minishell *mshell, t_cmd *cmd, int *pip);
void	wait_all(t_minishell *mshell);
bool	exec(t_minishell *mshell);
int	count_arg(char **params);
void	error_malloc(void);
void	update_oldpwd(t_minishell *mshell);
void	update_pwd(t_minishell *mshell, char *param);
int	ft_cd(t_minishell *mshell, char **params);
int	ft_env(t_lst *env);
int	check_new_line(char *str);
void	write_echo(int count, int i, bool new_line, char **args);
int	ft_echo(char **args);
int	almost_atoi(char *str, int *err);
void	ft_exit(t_minishell *mshell, char **args);
int	ft_pwd(void);
bool	export_no_args(t_lst *env);
bool	valid_identifier(char *str);
int	exist(char *str, t_lst *env);
bool	export(char *str, t_lst **env);
int	ft_export(char **str, t_lst **env);
bool	syntax(char *str);
void	check_env(t_lst *tmp, t_lst **env);
bool	unset(char *str, t_lst **env);
int	ft_unset(char **str, t_lst **env);
void	exec_builtin(int save_stdout, t_minishell *mshell, t_cmd *cmd);
bool	launch_builtin(t_minishell *mshell, t_cmd *cmd);
bool	check_dir(char **path, char *cmd, t_minishell *mshell);
bool	cmd_exist(char **path, t_minishell *mshell, char *cmd);
void	redirect_in_out(t_minishell *mshell, t_cmd *cmd, int *pip);
void	built(int *pip, t_cmd *cmd, t_minishell *mshell);
void	child_process(t_minishell *mshell, t_cmd *cmd, int *pip);
int	ft_strslashjoin(char *dest, char *str, char *env, int *index);
char	*create_paths(t_lst *env, int len);
char	*cmd_not_found(char *sample);
char	*find_cmd(t_minishell *mshell, char *sample, t_lst *env);
void	absolute_path(char **path, char *cmd, t_minishell *mshell);
char	**lst_to_arr(t_lst *env);
void	ft_swap_str_tab(int i, int j, char **tab);
void	sort_array(char **arr, int len);
size_t	len_cmd(t_cmd *list);
void	free_array(char **arr);
void	free_all(t_minishell *mshell, char *err, int ext);
void	free_all_cmd(t_cmd *tmp);
void	free_cmd(t_cmd **list);
void	*free_cmd_param(char **cmd, int i);

void	*free_cmd_param(char **cmd, int i)
{
	while (--i != -1)
		free(cmd[i]);
	free(cmd);
	return (NULL);
}

void	free_all_cmd(t_cmd *tmp)
{
	if (tmp->infile > 0)
		close(tmp->infile);
	tmp->infile = -2;
	if (tmp->outfile > 0)
		close(tmp->outfile);
	tmp->outfile = -2;
	free_array(tmp->cmd_param);
}

void	free_cmd(t_cmd **list)
{
	t_cmd	*tmp;
	t_cmd	*current;

	if (!(*list))
		return ;
	current = *list;
	while (current->next != *list)
	{
		tmp = current;
		current = current->next;
		free_all_cmd(tmp);
		free(tmp);
	}
	free_all_cmd(current);
	free(current);
	*list = NULL;
}

void	free_array(char **arr)
{
	int	i;

	i = 0;
	while (arr && arr[i])
		free(arr[i++]);
	if (arr)
		free(arr);
	arr = NULL;
}

void	free_all(t_minishell *mshell, char *err, int ext)
{
	if (mshell->cmd)
		free_cmd(&mshell->cmd);
	if (mshell->token)
		free_token(&mshell->token);
	if (mshell->env)
		free_list(&mshell->env);
	if (mshell->pipefd[0] && mshell->pipefd[0] != -1)
		close(mshell->pipefd[0]);
	if (mshell->pipefd[1] && mshell->pipefd[1] != -1)
		close(mshell->pipefd[1]);
	if (err)
		/*print_error(err);*/
		error(err);
	rl_clear_history();
	if (!access(".heredoc.tmp", F_OK))
		unlink(".heredoc.tmp");
	if (ext != -1)
		exit(ext);
}

size_t	len_cmd(t_cmd *list)
{
	t_cmd	*tmp;
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

void	ft_swap_str_tab(int i, int j, char **tab)
{
	char	*temp;

	temp = tab[i];
	tab[i] = tab[j];
	tab[j] = temp;
}

//sorts array
void	sort_array(char **arr, int len)
{
	int	i;
	int	j;
	int	diff;

	i = 0;
	while (i < len)
	{
		j = i + 1;
		while (j < len)
		{
			diff = ft_strncmp(arr[i], arr[j], __INT_MAX__);
			if (diff > 0)
			{
				ft_swap_str_tab(i, j, arr);
				continue ;
			}
			j++;
		}
	i++;
	}
}

char	**lst_to_arr(t_lst *env)
{
	t_lst	*lst;
	char	**dest;
	int		i;

	dest = NULL;
	i = 0;
	lst = env;
	dest = (char **)malloc(sizeof(char *) * (len_list(lst) + 1));
	if (!dest)
		return (NULL);
	dest[i] = (lst->str);
	lst = lst->next;
	i++;
	while (lst != env)
	{
		dest[i] = (lst->str);
		lst = lst->next;
		i++;
	}
	dest[i] = NULL;
	return (dest);
}

void	absolute_path(char **path, char *cmd, t_minishell *mshell)
{
	*path = ft_strdup(cmd);
	if (!(*path))
		free_all(mshell, ERR_MALLOC, EXT_MALLOC);
	if (access((*path), F_OK))
	{
		write(2, (*path), ft_strlen((*path)));
		write(2, " : command not found\n", 21);
		free(*path);
		*path = NULL;
	}
}

int	ft_strslashjoin(char *dest, char *str, char *env, int *index)
{
	int			i;
	int			j;

	i = 0;
	while (*index < (PATH_MAX - 1) && env[(*index)] && env[(*index)] != ':')
		dest[i++] = env[(*index)++];
	++(*index);
	dest[i++] = '/';
	j = 0;
	while (j < (PATH_MAX - 1) && str[j])
		dest[i++] = str[j++];
	dest[i] = '\0';
	return (0);
}


char	*create_paths(t_lst *env, int len)
{
	t_lst	*tmp;

	tmp = env;
	while (len--)
	{
		if (ft_strncmp(tmp->str, "PATH=", 5) == 0)
			return (&(tmp->str[5]));
		tmp = tmp->next;
	}
	return (NULL);
}

char	*cmd_not_found(char *sample)
{
	write(2, sample, ft_strlen(sample));
	write(2, " : command not found\n", 21);
	return (NULL);
}

char	*find_cmd(t_minishell *mshell, char *sample, t_lst *env)
{
	char		*paths;
	char		path[PATH_MAX];
	int			i;
	int			len;

	paths = create_paths(env, len_list(env));
	if (!paths || ft_strlen(sample) > PATH_MAX / 2)
		return (cmd_not_found(sample));
	i = 0;
	len = ft_strlen(paths);
	while (i < len)
	{
		ft_strslashjoin(path, sample, paths, &i);
		if (access(path, F_OK) == 0)
		{
			sample = ft_strdup(path);
			if (!sample)
			{
				/*print_error(ERR_MALLOC);*/
				error("Malloc error");
				mshell->exit_code = -1;
			}
			return (sample);
		}
	}
	return (cmd_not_found(sample));
}

bool	check_dir(char **path, char *cmd, t_minishell *mshell)
{
	struct stat	path_stat;

	stat(*path, &path_stat);
	if (!S_ISREG(path_stat.st_mode))
	{
		/*print_error(cmd);*/
		/*print_error(" : Is a directory\n");*/
		error(cmd);
		error(" : Is a directory\n");
		mshell->exit_code = 126;
		return (false);
	}
	return (true);
}

bool	cmd_exist(char **path, t_minishell *mshell, char *cmd)
{
	if (!ft_strchr(cmd, '/'))
		*path = find_cmd(mshell, cmd, mshell->env);
	else
		absolute_path(path, cmd, mshell);
	if (!(*path) && mshell->exit_code == -1)
		free_all(mshell, NULL, mshell->exit_code);
	if (!(*path))
	{
		mshell->exit_code = 127;
		return (false);
	}
	if (access((*path), X_OK))
	{
		perror(*path);
		free((*path));
		(*path) = NULL;
		mshell->exit_code = 126;
		return (false);
	}
	if (!check_dir(path, cmd, mshell))
		return (false);
	return (true);
}

void	redirect_in_out(t_minishell *mshell, t_cmd *cmd, int *pip)
{
	close(pip[0]);
	if (cmd->infile >= 0)
	{
		dup2(cmd->infile, 0);
		close(cmd->infile);
	}
	if (cmd->outfile >= 0)
	{
		dup2(cmd->outfile, 1);
		close(cmd->outfile);
	}
	else if (cmd->next != mshell->cmd)
		dup2(pip[1], 1);
	close(pip[1]);
}

void	built(int *pip, t_cmd *cmd, t_minishell *mshell)
{
	close(pip[0]);
	if (cmd->outfile < 0 && cmd->next != mshell->cmd)
		cmd->outfile = pip[1];
	else
		close(pip[1]);
	launch_builtin(mshell, cmd);
}

void	child_process(t_minishell *mshell, t_cmd *cmd, int *pip)
{
	char	*path;
	char	**env;

	path = NULL;
	if (cmd->skip_cmd)
		mshell->exit_code = 1;
	else if (is_builtin(cmd->cmd_param[0]))
		built(pip, cmd, mshell);
	else if (cmd_exist(&path, mshell, cmd->cmd_param[0]))
	{
		redirect_in_out(mshell, cmd, pip);
		env = lst_to_arr(mshell->env);
		if (!env)
			free_all(mshell, ERR_MALLOC, EXT_MALLOC);
		rl_clear_history();
		/*signals2();*/
		execve(path, cmd->cmd_param, env);
		free(env);
	}
	if (path)
		free(path);
	free_all(mshell, NULL, mshell->exit_code);
}

void	exec_builtin(int save_stdout, t_minishell *mshell, t_cmd *cmd)
{
	if (!ft_strncmp("echo", cmd->cmd_param[0], INT_MAX))
		mshell->exit_code = ft_echo(cmd->cmd_param);
	else if (!ft_strncmp("cd", cmd->cmd_param[0], INT_MAX))
		mshell->exit_code = ft_cd(mshell, cmd->cmd_param);
	else if (!ft_strncmp("pwd", cmd->cmd_param[0], INT_MAX))
		mshell->exit_code = ft_pwd();
	else if (!ft_strncmp("export", cmd->cmd_param[0], INT_MAX))
		mshell->exit_code = ft_export(cmd->cmd_param, &mshell->env);
	else if (!ft_strncmp("unset", cmd->cmd_param[0], INT_MAX))
		mshell->exit_code = ft_unset(cmd->cmd_param, &mshell->env);
	else if (!ft_strncmp("env", cmd->cmd_param[0], INT_MAX))
		mshell->exit_code = ft_env(mshell->env);
	else if (!ft_strncmp("exit", cmd->cmd_param[0], INT_MAX))
	{
		if (cmd->outfile >= 0)
		{
			dup2(save_stdout, 1);
			close(save_stdout);
		}
		ft_exit(mshell, cmd->cmd_param);
	}
}

bool	launch_builtin(t_minishell *mshell, t_cmd *cmd)
{
	int	save_stdout;

	save_stdout = -1;
	if (cmd->outfile >= 0)
	{
		save_stdout = dup(1);
		dup2(cmd->outfile, 1);
	}
	exec_builtin(save_stdout, mshell, cmd);
	if (cmd->outfile >= 0)
	{
		dup2(save_stdout, 1);
		close (save_stdout);
	}
	return (true);
}

//syntax
bool	syntax(char *str)
{
	int	i;

	if (str[0] != '_' && !ft_isalpha(str[0]))
		return (false);
	i = 0;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

//checks if identifier already in env
int	exist(char *str, t_lst *env)
{
	int		i;
	int		j;
	t_lst	*tmp;

	if (!env)
		return (-1);
	i = 0;
	while (str[i])
		i++;
	j = 0;
	tmp = env;
	if (!ft_strncmp(tmp->str, str, i))
		return (j);
	tmp = tmp->next;
	j++;
	while (tmp != env)
	{
		if (!ft_strncmp(tmp->str, str, i))
			return (j);
		tmp = tmp->next;
		j++;
	}
	return (-1);
}

void	check_env(t_lst *tmp, t_lst **env)
{
	if (tmp == (*env))
		(*env) = tmp->next;
	if (tmp->next == tmp)
		(*env) = NULL;
}

bool	unset(char *str, t_lst **env)
{
	int		pos;
	int		i;
	t_lst	*tmp;

	if (!str || !(*str))
		return (false);
	if (!syntax(str))
	{
		/*print_error("unset: invalid identifier\n");*/
		error("unset: invalid identifier");
		return (true);
	}
	pos = exist(str, (*env));
	if (pos == -1)
		return (false);
	tmp = (*env);
	i = 0;
	while (i++ < pos)
		tmp = tmp->next;
	tmp->prev->next = tmp->next;
	tmp->next->prev = tmp->prev;
	free(tmp->str);
	check_env(tmp, env);
	free(tmp);
	tmp = NULL;
	return (false);
}

int	ft_unset(char **str, t_lst **env)
{
	int	exit_code;
	int	i;

	exit_code = 0;
	i = 0;
	while (str[i])
	{
		if (unset(str[i], env))
			exit_code = 1;
		i++;
	}
	return (exit_code);
}

//if export and no other args
bool	export_no_args(t_lst *env)
{
	char	**arr;
	int		i;
	int		j;

	arr = lst_to_arr(env);
	if (!arr)
		return (false);
	sort_array(arr, len_list(env));
	i = 0;
	while (arr[i])
	{
		printf("declare -x ");
		j = 0;
		while (arr[i][j] && arr[i][j] != '=')
			printf("%c", arr[i][j++]);
		if (arr[i][j] && arr[i][j] == '=')
			printf("=\"%s\"\n", &arr[i][j + 1]);
		else
			printf("\n");
		i++;
	}
	free(arr);
	return (true);
}

//checks syntax
bool	valid_identifier(char *str)
{
	int	i;

	i = 0;
	if (!str[0] || (str[0] != '_' && !ft_isalpha(str[0])))
		return (false);
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

//export but norm
bool	export(char *str, t_lst **env)
{
	int		pos;
	int		i;
	char	*value;

	pos = exist(str, (*env));
	value = ft_strdup(str);
	if (!value)
		return (false);
	if (pos >= 0)
	{
		i = 0;
		while (i < pos)
		{
			(*env) = (*env)->next;
			i++;
		}
		free((*env)->str);
		(*env)->str = value;
	}
	else if (pos == -1)
		if (!append(env, value))
			return (false);
	return (true);
}

//export
int	ft_export(char **str, t_lst **env)
{
	int	exit_code;
	int	i;

	exit_code = 0;
	i = 1;
	if (!str || !str[i])
	{
		if (*env && !export_no_args((*env)))
			/*return (print_error(ERR_MALLOC));*/
			return (69);
		return (0);
	}
	while (str[i])
	{
		if (!valid_identifier(str[i]))
		{
			/*print_error("export: invalid identifier\n");*/
			error("export: invalid identifier\n");
			exit_code = 1;
		}
		else if (!export(str[i], env))
			/*return (print_error(ERR_MALLOC));*/
			return (69);
		i++;
	}
	return (exit_code);
}

int	ft_pwd(void)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, PATH_MAX))
	{
		printf("%s\n", cwd);
		return (0);
	}
	else
	{
		perror("pwd");
		return (1);
	}
}

int	almost_atoi(char *str, int *err)
{
	unsigned long long	ret;
	int					i;
	int					j;
	int					pn;

	i = 0;
	while ((9 <= str[i] && str[i] <= 13) || str[i] == 32)
		i++;
	pn = 1;
	if (str[i] == '+' || str[i] == '-')
		if (str[i++] == '-')
			pn = -1;
	j = i;
	ret = 0;
	while ('0' <= str[i] && str[i] <= '9')
		ret = ret * 10 + (str[i++] - 48);
	while ((9 <= str[i] && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] || i - j > 20 || ((pn == -1 && (ret - 1) > LONG_MAX) || \
		(pn == 1 && (ret > LONG_MAX))))
		*err = 1;
	return ((int)((ret * pn) % 256));
}

void	ft_exit(t_minishell *mshell, char **args)
{
	int	ret;
	int	err;

	ret = 0;
	err = 0;
	if (args[1])
	{
		ret = almost_atoi(args[1], &err);
		if (err)
		{
			/*print_error("exit: ");*/
			/*print_error(args[1]);*/
			/*print_error(": numeric argument required\n");*/
			error("exit: ");
			error(args[1]);
			error(": numeric argument required\n");
			free_all(mshell, NULL, 2);
		}
	}
	if (args[1] && args[2])
	{
		/*print_error("exit: too many arguments\n");*/
		error("exit: too many arguments\n");
		mshell->exit_code = 1;
		return ;
	}
	if (!args[1])
		free_all(mshell, NULL, mshell->exit_code);
	free_all(mshell, NULL, ret);
}

int	ft_env(t_lst *env)
{
	t_lst	*temp;

	temp = env;
	if (!temp)
		return (0);
	if (ft_strchr(temp->str, '='))
		printf("%s\n", temp->str);
	temp = temp->next;
	while (temp != env)
	{
		if (ft_strchr(temp->str, '='))
			printf("%s\n", temp->str);
		temp = temp->next;
	}
	return (0);
}

int	check_new_line(char *str)
{
	int	i;

	i = 0;
	if (str[i] && str[i] == '-')
	{
		++i;
		while (str[i] && str[i] == 'n')
			i++;
		if (i == (int)ft_strlen(str))
			return (1);
	}
	return (0);
}

void	write_echo(int count, int i, bool new_line, char **args)
{
	while (args[i] && check_new_line(args[i]))
	{
		++i;
		new_line = false;
	}
	while (i < count)
	{
		write(1, args[i], ft_strlen(args[i]));
		if (i != count - 1)
			write(1, " ", 1);
		++i;
	}
	if (new_line)
		write(1, "\n", 1);
}

int	ft_echo(char **args)
{
	int		count;
	int		i;
	bool	new_line;

	count = 0;
	while (args[count])
		++count;
	i = 1;
	new_line = true;
	write_echo(count, i, new_line, args);
	return (0);
}

int	count_arg(char **params)
{
	int	count;

	count = 0;
	while (params[count])
		count++;
	return (count);
}

void	error_malloc(void)
{
	/*print_error(ERR_MALLOC);*/
	error("Malloc error");
	return ;
}

void	update_oldpwd(t_minishell *mshell)
{
	t_lst	*tmp;
	char	*test;
	int		len;

	tmp = mshell->env;
	test = NULL;
	len = len_list(tmp);
	while (len--)
	{
		if (ft_strncmp(tmp->str, "PWD=", 3) == 0)
			test = tmp->str;
		tmp = tmp->next;
	}
	if (!test)
		export("OLDPWD", &mshell->env);
	if (test)
	{
		test = ft_strjoin("OLD", test);
		if (!test)
			return (error_malloc());
		export(test, &mshell->env);
	}
	free(test);
}

void	update_pwd(t_minishell *mshell, char *param)
{
	char	cwd[PATH_MAX];
	char	*pwd;

	update_oldpwd(mshell);
	if (getcwd(cwd, PATH_MAX) == NULL)
	{
		perror(param);
		return ;
	}
	pwd = ft_strjoin("PWD=", cwd);
	if (!pwd)
		return (error_malloc());
	export(pwd, &mshell->env);
	free(pwd);
}

int	ft_cd(t_minishell *mshell, char **params)
{
	int	res;

	if (count_arg(params) == 2)
	{
		res = chdir(params[1]);
		if (res == 0)
			update_pwd(mshell, params[1]);
		if (res == -1)
			res *= -1;
		if (res == 1)
			perror(params[1]);
		return (res);
	}
	return (1);
}

bool	is_builtin(char *cmd)
{
	if (!cmd)
		return (false);
	if (!ft_strncmp("echo", cmd, INT_MAX) || !ft_strncmp("cd", cmd, INT_MAX) \
	|| !ft_strncmp("pwd", cmd, INT_MAX) || !ft_strncmp("export", cmd, INT_MAX) \
	|| !ft_strncmp("unset", cmd, INT_MAX) || !ft_strncmp("env", cmd, INT_MAX) \
	|| !ft_strncmp("exit", cmd, INT_MAX))
		return (true);
	return (false);
}

void	parent_process(t_minishell *mshell, t_cmd *cmd, int *pip)
{
	close(pip[1]);
	if (cmd->infile >= 0)
		close(cmd->infile);
	if (cmd->infile == -2)
		cmd->infile = pip[0];
	if (cmd->next != mshell->cmd && cmd->next->infile == -2)
		cmd->next->infile = pip[0];
	else
		close(pip[0]);
}

bool	exec_cmd(t_minishell *mshell, t_cmd *cmd, int *pip)
{
	g_signal_pid = fork();
	if (g_signal_pid < 0)
		free_all(mshell, ERR_FORK, EXT_FORK);
	else if (!g_signal_pid)
	{
		if (cmd->cmd_param && cmd->cmd_param[0])
			child_process(mshell, cmd, pip);
		else
			free_all(mshell, NULL, 0);
	}
	else
		parent_process(mshell, cmd, pip);
	return (true);
}

void	wait_all(t_minishell *mshell)
{
	int		status;
	int		pid;
	int		len;
	t_cmd	*tmp;

	tmp = mshell->cmd;
	len = len_cmd(tmp);
	while (len--)
	{
		pid = waitpid(0, &status, 0);
		if (pid == g_signal_pid)
		{
			if (WIFEXITED(status))
				mshell->exit_code = WEXITSTATUS(status);
		}
		if (tmp->outfile >= 0)
			close(tmp->outfile);
		if (tmp->infile >= 0)
			close(tmp->infile);
		tmp = tmp->next;
	}
}

bool	exec(t_minishell *mshell)
{
	t_cmd	*tmp;
	int		*pip;

	pip = mshell->pipefd;
	tmp = mshell->cmd;
	if (tmp && tmp->skip_cmd == false && tmp->next == tmp && tmp->cmd_param[0] \
		&& is_builtin(tmp->cmd_param[0]))
		return (launch_builtin(mshell, tmp));
	if (pipe(pip) == -1)
		return (false);
	exec_cmd(mshell, tmp, pip);
	tmp = tmp->next;
	while (tmp != mshell->cmd)
	{
		if (pipe(pip) == -1)
			return (-1);
		exec_cmd(mshell, tmp, pip);
		tmp = tmp->next;
	}
	wait_all(mshell);
	return (true);
}

bool	check_pipe(t_minishell *mshell)
{
	if (mshell->token->id == PIPE)
	{
		write(2, "syntax error near unexpected token '|'\n", 39);
		free_token(&mshell->token);
		free_cmd(&mshell->cmd);
		return (false);
	}
	return (true);
}

bool	read_in_stdin(t_minishell *mshell, int fd, char *word)
{
	char	*buf;

	while (1)
	{
		buf = NULL;
		buf = readline("> ");
		if (!buf)
		{
			/*print_error("warning: here-document delimited by end-of-file ");*/
			/*print_error("(wanted '");*/
			/*print_error(word);*/
			/*print_error("')\n");*/
			error("warning: here-document delimited by end-of-file ");
			error("(wanted '");
			error(word);
			error("')\n");
			break ;
		}
		if (!ft_strncmp(word, buf, INT_MAX))
			break ;
		if (!replace_dollar(&buf, mshell))
			free_all(mshell, ERR_MALLOC, EXT_MALLOC);
		write(fd, buf, ft_strlen(buf));
		write(fd, "\n", 1);
		free(buf);
	}
	free(buf);
	close(fd);
	return (true);
}

int	here_doc(t_minishell *mshell, char *word)
{
	int	fd;

	fd = open(".heredoc.tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		return (-1);
	if (!read_in_stdin(mshell, fd, word))
	{
		unlink(".heredoc.tmp");
		return (-1);
	}
	fd = open(".heredoc.tmp", O_RDONLY);
	if (fd > 0)
		unlink(".heredoc.tmp");
	return (fd);
}

bool	print_error_token(t_token *token, t_minishell *data)
{
	write(2, "syntax error near unexpected token ", 35);
	write(2, "'", 1);
	if (token->next == data->token)
		write(2, "newline", 7);
	else
		write(2, token->next->text, ft_strlen(token->next->text));
	write(2, "'\n", 2);
	return (false);
}

int	open_file(t_minishell *data, char *filename, int type)
{
	int	fd;

	fd = -2;
	if (type == INPUT)
		fd = open(filename, O_RDONLY, 0644);
	else if (type == HEREDOC)
		fd = here_doc(data, filename);
	else if (type == TRUNC)
		fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else if (type == APPEND)
		fd = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (type != HEREDOC && fd < 0)
		perror(filename);
	return (fd);
}

bool	get_in(t_minishell *data, t_token *tmp, t_cmd *cmd)
{
	if (tmp->id == INPUT)
	{
		if (cmd->infile >= 0)
			close(cmd->infile);
		if (tmp == tmp->next || tmp->next->id <= 5)
			return (print_error_token(tmp, data));
		cmd->infile = open_file(data, tmp->next->text, INPUT);
		if (cmd->infile == -1)
			return (false);
	}
	else if (tmp->id == HEREDOC)
	{
		if (cmd->infile >= 0)
			close(cmd->infile);
		if (tmp == tmp->next || tmp->next->id <= 5)
			return (print_error_token(tmp, data));
		cmd->infile = open_file(data, tmp->next->text, HEREDOC);
		if (cmd->infile == -1)
			return (false);
	}
	return (true);
}

bool	get_infile(t_minishell *data, t_token *token, t_cmd *cmd)
{
	t_token	*tmp;

	tmp = token;
	if (tmp->id != PIPE && !get_in(data, tmp, cmd))
		return (false);
	if (tmp->id == PIPE)
		return (true);
	tmp = tmp->next;
	while (tmp->id != PIPE && tmp != data->token)
	{
		if (!get_in(data, tmp, cmd))
			return (false);
		tmp = tmp->next;
	}
	return (true);
}

bool	get_out(t_token *tmp, t_cmd *cmd, t_minishell *data)
{
	if (tmp->id == TRUNC)
	{
		if (cmd->outfile >= 0)
			close(cmd->outfile);
		if (tmp == tmp->next || tmp->next->id <= 5)
			return (print_error_token(tmp, data));
		cmd->outfile = open_file(NULL, tmp->next->text, TRUNC);
		if (cmd->outfile == -1)
			return (false);
	}
	else if (tmp->id == APPEND)
	{
		if (cmd->outfile >= 0)
			close(cmd->outfile);
		if (tmp == tmp->next || tmp->next->id <= 5)
			return (print_error_token(tmp, data));
		cmd->outfile = open_file(NULL, tmp->next->text, APPEND);
		if (cmd->outfile == -1)
			return (false);
	}
	return (true);
}

bool	get_outfile(t_token *token, t_cmd *cmd, t_minishell *data)
{
	t_token	*tmp;

	tmp = token;
	if (tmp->id != PIPE && !get_out(tmp, cmd, data))
		return (false);
	tmp = tmp->next;
	while (tmp != data->token && tmp->id != PIPE)
	{
		if (!get_out(tmp, cmd, data))
			return (false);
		tmp = tmp->next;
	}
	return (true);
}

int	count_args(t_minishell *data, t_token *token)
{
	int		count;
	t_token	*tmp;

	count = 0;
	tmp = token;
	if (tmp->id == CMD || (tmp->id == ARG && \
		tmp->prev != data->token->prev && tmp->prev->id > 5))
		count ++;
	tmp = tmp->next;
	while (tmp != data->token && tmp->id != PIPE)
	{
		if (tmp->id == CMD || (tmp->id == ARG && \
		tmp->prev != data->token->prev && tmp->prev->id > 5))
			count ++;
		tmp = tmp->next;
	}
	return (count);
}

int	add_to_cmd_param(char **cmd_param, int *i, char *str)
{
	cmd_param[*i] = ft_strdup(str);
	if (!cmd_param[*i])
		return (0);
	(*i)++;
	return (1);
}


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
	if (tmp->id != PIPE && (tmp->id == CMD || (tmp->id == ARG && \
		tmp->prev != mshell->token->prev && tmp->prev->id > 5)) && \
		!add_to_cmd_param(cmd_param, &i, tmp->text))
		return (free_cmd_param(cmd_param, i));
	tmp = tmp->next;
	while (tmp != mshell->token && tmp->id != PIPE)
	{
		if ((tmp->id == CMD || (tmp->id == ARG && \
			tmp->prev != mshell->token->prev && tmp->prev->id > 5)) && \
			!add_to_cmd_param(cmd_param, &i, tmp->text))
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
		free_all(mshell, ERR_MALLOC, EXT_MALLOC);
	return (true);
}

bool	norm(t_minishell *mshell, t_token *tmp)
{
	if (!append_cmd(&mshell->cmd, -2, -2, NULL))
		free_all(mshell, ERR_MALLOC, EXT_MALLOC);
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
		if (tmp->prev->id == PIPE)
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
	if (mshell->token && mshell->token->prev->id == PIPE)
	{
		write(2, "Error: Unclosed pipe\n", 21);
		mshell->exit_code = 2;
		free_token(&mshell->token);
		return (false);
	}
	if (mshell->token)
		create_list_cmd(mshell); // TODO
	return (check_pipe(mshell));
	/*if (mshell->token)*/
		/*create_list_cmd(mshell); // TODO*/
	/*if (!mshell->token || !create_list_cmd(mshell))*/
	/*{*/
		/*free_token(&mshell->token);*/
		/*free_cmd(&mshell->cmd);*/
		/*return (false);*/
	/*}*/
	/*return (check_pipe(mshell));*/
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
	g_signal_pid = 0;
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
			exec(&mshell);
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
