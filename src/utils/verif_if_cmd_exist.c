#include <minishell.h>

bool	insert_builtins(char **builtins)
{
	builtins[0] = ft_strdup("echo");
	if (!builtins[0])
		return (false);
	builtins[1] = ft_strdup("cd");
	if (!builtins[1])
		return (false);
	builtins[2] = ft_strdup("pwd");
	if (!builtins[2])
		return (false);
	builtins[3] = ft_strdup("export");
	if (!builtins[3])
		return (false);
	builtins[4] = ft_strdup("unset");
	if (!builtins[4])
		return (false);
	builtins[5] = ft_strdup("env");
	if (!builtins[5])
		return (false);
	builtins[6] = ft_strdup("exit");
	if (!builtins[6])
		return (false);
	builtins[7] = NULL;
	return (true);
}

bool	verif_inside(char *directory, char *cmd)
{
	DIR				*dirp;
	struct dirent	*dirr;

	dirp = opendir(directory);
	if (!dirp)
		return (false);
	dirr = readdir(dirp);
	while (dirr)
	{
		if (!ft_strncmp(cmd, dirr->d_name, 69))
			return (true);
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
	if (!arr)
		return (false);
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
		if (!arr)
			return (false);
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
	if (!bins_dir)
		return (2);
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
		if (!ft_strncmp(needle, haystack[i], 69))
			return (true);
		++i;
	}
	return (false);
}

t_verif	*validate(t_token *token_root, char *path, char **builtins)
{
	t_verif	*result;

	result = malloc(sizeof(t_verif));
	if (!result)
		return (NULL);
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
					if (!result->name)
						return (NULL);
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
