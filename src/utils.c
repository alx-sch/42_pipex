/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aschenk <aschenk@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 18:44:40 by aschenk           #+#    #+#             */
/*   Updated: 2024/03/07 13:11:43 by aschenk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "libft/libft.h"

// libft
void	ft_putchar_fd(char c, int fd);
char	*ft_substr(const char *s, unsigned int start, size_t len);

//	+++++++++++++++
//	++ FUNCTIONS ++
//	+++++++++++++++

void	perror_and_exit(char *message)
{
	perror(message);
	exit(EXIT_FAILURE);
}


// get variable name in substring (all before '=')
// returns everthing behind '='
char	*ft_getenv(const char *name, char *env[])
{
	int		i;
	int		j;
	char	*env_var_name;

	i = 0;
	while (env[i])
	{
		j = 0;
		while (env[i][j] != '=' && env[i][j])
			j++;
		env_var_name = ft_substr(env[i], 0, j);
		if (ft_strcmp(env_var_name, name) == 0)
		{
			free(env_var_name);
			return (env[i] + j + 1);
		}
		free(env_var_name);
		i++;
	}
	return (NULL);
}


char	*get_cmd_path(char *cmd, char **env)
{
	int		i;
	char	*exec;
	char	*allpath[];
	char	*path_part;
	char	*s_cmd[];

	i = -1;
	allpath = ft_split(ft_getenv("PATH", env), ':');
	s_cmd = ft_split(cmd, ' ');
	while (allpath[++i])
	{
		path_part = ft_strjoin(allpath[i], "/");
		exec = ft_strjoin(path_part, s_cmd[0]);
		free(path_part);
		if (access(exec, F_OK | X_OK) == 0)
		{
			ft_free_tab(s_cmd);
			return (exec);
		}
		free(exec);
	}
	ft_free_tab(allpath);
	ft_free_tab(s_cmd);
	return (cmd);
}


// void	execute(char *cmd, char *env[])
// {
// 	char	*cmd_args[];
// 	char	*cmd_path;

// 	cmd_args = ft_split(cmd, ' ');
// 	cmd_path = get_cmd_path(cmd_args[0], env);
// 	if (execve(path, cmd_args, env) == -1)
// 	{
// 		ft_putstr_fd("pipex: command not found: ", 2);
// 		ft_putendl_fd(s_cmd[0], 2);
// 		ft_free_tab(s_cmd);
// 		exit(0);
// 	}
// }