/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aschenk <aschenk@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 18:44:40 by aschenk           #+#    #+#             */
/*   Updated: 2024/03/07 20:22:00 by aschenk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "libft/libft.h"

// FILE
void	perror_and_exit(const char *msg);

// libft
void	ft_putstr_fd(char *s, int fd);
char	*ft_substr(const char *s, unsigned int start, size_t len);
int		ft_strcmp(const char *s1, const char *s2);

//	+++++++++++++++
//	++ FUNCTIONS ++
//	+++++++++++++++

void	perror_and_exit(const char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

void	msg_and_exit(char *msg)
{
	ft_putstr_fd(msg, STDERR_FILENO);
	exit(EXIT_FAILURE);
}

// void	free_arr(char **array)
// {
// 	size_t	i;

// 	i = 0;
// 	if (!array)
// 		return ;
// 	while (array[i])
// 	{
// 		free(array[i]);
// 		i++;
// 	}
// 	free(array);
// }

// // get variable name in substring (all before '=') (env_var)
// // returns everthing behind '=', the vlaue of env var
char	*ft_getenv(const char *env_var_search, char **env)
{
	size_t	i;
	int		len;
	char	*env_var;

	i = 0;
	while (env[i])
	{
		len = 0;
		while (env[i][len] != '=' && env[i][len])
			len++;
		env_var = ft_substr(env[i], 0, len);
		if (!env_var)
			exit(EXIT_FAILURE);
		if (ft_strcmp(env_var, env_var_search) == 0)
		{
			free(env_var);
			return (env[i] + len + 1);
		}
		free(env_var);
		i++;
	}
	return (NULL);
}

// char	*get_cmd_path(char *cmd, char **env)
// {
// 	int		i;
// 	char	**directories;
// 	char	**cmd_tokens;
// 	char	*exec_path;

// 	i = 0;
// 	directories = ft_split(ft_getenv("PATH", env), ':');
// 	cmd_tokens = ft_split(cmd, ' ');
// 	while (directories[i])
// 	{
// 		exec_path = ft_strjoin(directories[i], cmd_tokens[0]);
// 		if (access(exec_path, F_OK | X_OK) == 0)
// 		{
// 			free_arr(cmd_tokens);
// 			return (exec_path);
// 		}
// 		free(exec_path);
// 		i++;
// 	}
// 	free_arr(directories);
// 	free_arr(cmd_tokens);
// 	return (cmd);
// }

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
