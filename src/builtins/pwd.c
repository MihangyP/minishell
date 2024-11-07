/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irazafim <irazafim@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 09:22:15 by irazafim          #+#    #+#             */
/*   Updated: 2024/11/07 12:34:02 by pmihangy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void pwd_minishell (void)
{
	char cwd[1024];

	getcwd(cwd, sizeof(cwd));
	printf("%s\n", cwd);
}
