#include <minishell.h>

bool	insert_builtins(char **builtins)
{
	builtins[0] = ft_strdup("echo");
	IF_RETURN(!builtins[0], false)
	builtins[1] = ft_strdup("cd");
	IF_RETURN(!builtins[1], false)
	builtins[2] = ft_strdup("pwd");
	IF_RETURN(!builtins[2], false)
	builtins[3] = ft_strdup("export");
	IF_RETURN(!builtins[3], false)
	builtins[4] = ft_strdup("unset");
	IF_RETURN(!builtins[4], false)
	builtins[5] = ft_strdup("env");
	IF_RETURN(!builtins[5], false)
	builtins[6] = ft_strdup("exit");
	IF_RETURN(!builtins[6], false)
	builtins[7] = NULL;
	return (true);
}

bool	verif_inside(char *directory, char *cmd)
{
	DIR				*dirp;
	struct dirent	*dirr;

	dirp = opendir(directory);
	IF_RETURN(!dirp, false)
	dirr = readdir(dirp);
	while (dirr)
	{
		IF_RETURN(!ft_strncmp(cmd, dirr->d_name, 69), true)
		dirr = readdir(dirp);
	}
	closedir(dirp);
	return (false);
}

bool	is_valide_path(char *cmd, char *path)
{
	char	**arr;
	int		i;
	int		j;
	int		counter;
	int		len;
	
	arr = ft_split(path, ':');
	IF_RETURN(!arr, false)
	j = ft_strlen(cmd) - 1;
	counter = 0;
	while (cmd[j] != '/')
	{
		++counter;
		--j;	
	}
	++counter;
	len = ft_strlen(cmd) - counter;
	if (len == 0)
		return (false);
	i = 0;
	while (arr[i])
	{
		if (!ft_strncmp(cmd, arr[i], len))
			return (true);
		++i;
	}
	return (false);
}

bool	verif_if_absolute_path(char *directory, char *path, char *cmd)
{
	char	**arr;
	int		i;

	if (is_valide_path(cmd, path))
	{
		arr = ft_split(cmd, '/');
		IF_RETURN(!arr, false)
			i = 0;
		while (arr[i + 1] != NULL)
			++i;
		if (verif_inside(directory, arr[i]))
			return (true);
	}
	return (false);
}

size_t	verify_if_in_path(char *cmd, char *path)
{
	char	**bins_dir;
	int			i;

	bins_dir = ft_split(path, ':');
	IF_RETURN(!bins_dir, 2)
	i = 0;
	while (bins_dir[i])
	{
		if (verif_inside(bins_dir[i], cmd))
			return (1);
		else if (verif_if_absolute_path(bins_dir[i], path, cmd))
			return (1);
		++i;
	}
	return (0);
}

bool	is_in(char *needle, char **haystack)
{
	int	i;

	i = 0;
	while (haystack[i])
	{
		IF_RETURN(!ft_strncmp(needle, haystack[i], 69), true)
		++i;
	}
	return (false);
}

t_verif	*validate(t_token *token_root, char *path, char **builtins)
{
	t_verif	*result;

	result = malloc(sizeof(t_verif));
	IF_RETURN(!result, NULL)
	result->res = true;
	result->name = NULL;
	while (token_root)
	{
		if (token_root->identifier == CMD)
		{
			if (!is_in(token_root->text, builtins))
			{
				if (!verify_if_in_path(token_root->text, path))
				{
					result->res = false;
					result->name = ft_strdup(token_root->text);
					IF_RETURN(!result->name, NULL)
					return (result);
				}
				else if (verify_if_in_path(token_root->text, path) == 2)
					return (NULL);
			}
		}
		token_root = token_root->next;
	}
	return (result);
}
