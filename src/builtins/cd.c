/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irazafim <irazafim@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 09:57:02 by irazafim          #+#    #+#             */
/*   Updated: 2024/10/22 12:36:30 by pmihangy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

// TODO: set the return value to 0 if has an error, 1 else
int    cd_minishell(char *argv[], char ***env)
{
	char	old_pwd[1024];
	char	pwd[1024];

	getcwd(old_pwd, sizeof(old_pwd));
	if (chdir(argv[0]) == -1)
	{
		printf("Error: %s\n", strerror(errno));
		return (1);
	}
	else
	{
		getcwd(pwd, sizeof(pwd));
		IF_RETURN(!env_update(env, ft_strjoin("OLDPWD=", old_pwd)), 1)
		IF_RETURN(!env_update(env, ft_strjoin("PWD=", pwd)), 1)
	}
	return (0);
}
