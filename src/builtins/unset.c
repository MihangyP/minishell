/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmihangy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 10:40:46 by pmihangy          #+#    #+#             */
/*   Updated: 2024/10/22 09:22:24 by pmihangy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

size_t	count_new_env_size(char **env, char **to_unset)
{
	char	*name;
	size_t	counter;
	int		i;

	counter = 0;
	i = 0;
	while (env[i])
	{
		name = get_variable_name(env[i]);
		IF_RETURN(!name, 0)
		if (!is_in(name, to_unset))
			++counter;
		++i;
	}
	return (counter);
}

// TODO: check if has an error
bool	unset_minishell(char ***env, char **to_unset)
{
	int		i;
	int		j;
	char	**new_env;
	char	*name;
	size_t	size;

	size = count_new_env_size(*env, to_unset);
	IF_RETURN(size == 0, false)
	new_env = malloc((size + 1) * sizeof(char *));
	IF_RETURN(!new_env, false)
	i = 0;
	j = 0;
	while ((*env)[i])
	{
		name = get_variable_name((*env)[i]);
		IF_RETURN(!name, false)
		if (!is_in(name, to_unset))
		{
			new_env[j] = ft_strdup((*env)[i]);
			IF_RETURN(!new_env[j], false)
			++j;
		}
		++i;
	}
	new_env[j] = NULL;
	*env = new_env;
	return (true);
}
