/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmihangy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 12:13:11 by pmihangy          #+#    #+#             */
/*   Updated: 2024/11/07 17:31:52 by pmihangy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	handle_sigint(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\nminishell> ", 12);
}

void	handle_sigquit(int sig)
{
	(void)sig;
}

void	listen_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);
}

bool	repl(char ***env, char *path)
{
	char		*entry;
	t_token		*token_root;
	t_ast		*ast_root;
	/*char		**builtins;*/
	/*pid_t		pid;*/

	/*builtins = malloc(8 * sizeof(char *));*/
	/*if (!builtins)*/
		/*return (false);*/
	/*if (!insert_builtins(builtins))*/
		/*return (false);*/
	token_root = NULL;
	listen_signals();
	while (1)
	{
		entry = readline("minishell> ");
		if (entry == NULL)
		{
			printf("exit\n");
			return (false);
		}
		add_history(entry);
		if (has_open_quote(entry, false, 0))
			printf("open quote\n");
		else
		{
			// TODO: CLEAN THE CODE 
			lexer(&token_root, entry);
			print_tokens(token_root);
			/*verify(ast_root);*/
			/*parse(ast_root, token_root);*/
			/*execute(ast_root);*/
			// TODO: REVIEW OUR APPROCH
		}
		/*else*/
		/*{*/
			/*token_root = lexer(entry);*/
			/*if (token_root == NULL)*/
				/*return (free(entry), false);*/
			/*ast_root = parse(token_root);*/
			/*if (ast_root == NULL)*/
				/*return (free(entry), false);*/
			/*t_verif	*verify = validate(token_root, path, builtins);*/
			/*if (!verify)*/
				/*return (false);*/
			/*if (!verify->res)*/
				/*printf("command '%s' not found\n", verify->name);*/
			/*else*/
			/*{*/
				/*// TODO: exec builtins functions*/
				/*if (!ast_root->left && !ast_root->right)*/
				/*{*/
					/*if (is_in(ast_root->text, builtins))*/
					/*{*/
						/*if (!ft_strncmp(ast_root->text, "echo", 69))*/
							/*echo_minishell(ast_root->argv);*/
						/*else if (!ft_strncmp(ast_root->text, "env", 69))*/
						/*{*/
							/*if (ast_root->argv)*/
								/*printf("usage: env\n");*/
							/*else*/
								/*env_minishell(*env);*/
						/*}*/
						/*else if (!ft_strncmp(ast_root->text, "export", 69))*/
							/*export_minishell(env, ast_root->argv);	*/
						/*else if (!ft_strncmp(ast_root->text, "unset", 69))*/
							/*unset_minishell(env, ast_root->argv);	*/
						/*else if (!ft_strncmp(ast_root->text, "pwd", 69))*/
							/*pwd_minishell();	*/
						/*else if (!ft_strncmp(ast_root->text, "cd", 69))*/
							/*cd_minishell(ast_root->argv, env);	*/
						/*else if (!ft_strncmp(ast_root->text, "exit", 69))*/
							/*exit_minishell(ast_root->argv);	*/
					/*}*/
					/*else*/
					/*{*/
						/*pid = fork();*/
						/*if (pid == 0)*/
						/*{*/
							/*if (ft_strchr(ast_root->text, '/'))*/
								/*execve(ast_root->text, ft_split(entry, ' '), *env);*/
							/*else*/
							/*{*/
								/*char	*dir_path = get_dir_path(ast_root->text, path);*/
								/*if (!dir_path)*/
									/*return (false);*/
								/*execve(ft_strjoin(dir_path, ast_root->text), ft_split(entry, ' '), *env);*/
							/*}*/
						/*}*/
						/*else*/
							/*wait(NULL);*/
					/*}*/
				/*}*/
			/*}*/
		/*}*/
		free(entry);
	}
	return (true);
}

int	main(int ac, char **av, char **env)
{
	int		i;
	char	*path;

	(void)ac;
	(void)av;
	path = get_path(env);
	if (!repl(&env, path))
		return (1);
	return (0);
}
