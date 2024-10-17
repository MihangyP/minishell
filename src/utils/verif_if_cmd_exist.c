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

size_t	verify_if_in_path(char *cmd, char *path)
{
	char	**bins_dir;
	char		**tmp;
	int			i;

	bins_dir = ft_split(path, ':');
	IF_RETURN(!bins_dir, 2)
	i = 0;
	while (bins_dir[i])
	{
		IF_RETURN(verif_inside(bins_dir[i], cmd), 1)
		++i;
	}
	return (0);
}

bool	inside_builtins(char *cmd, char **builtins)
{
	int	i;

	i = 0;
	while (builtins[i])
	{
		IF_RETURN(!ft_strncmp(cmd, builtins[i], 69), true)
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
			if (!inside_builtins(token_root->text, builtins))
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
