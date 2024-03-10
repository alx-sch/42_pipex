/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aschenk <aschenk@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 00:39:57 by aschenk           #+#    #+#             */
/*   Updated: 2024/03/10 21:01:51 by aschenk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "libft/libft.h"

// FILE
void	call_cmd(char *cmd, char *env[]);

// utils.c
//char	*add_newline(const char *str);
void	free_arr(char **array);
void	errno_and_exit(const char *pre, const char *post);

// libft
void	ft_putstr_fd(char *s, int fd);

// // get variable name in substring (all before '=') (env_var)
// // returns everything behind '=', the vlaue of env var
// looking for env var "PATH", value: list of directories where OS looks
// for executables when command is typed into command line interface;
// each directory separated by colon (:) on UNIX-like systems
static char	*get_env_values(const char *env_var_search, char **env)
{
	size_t	i;
	size_t	len;
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

// returns system path of command executable (first apparence in PATH env var)
static char	*get_cmd_path(char *cmd, char **env)
{
	size_t	i;
	char	**directories;
	char	**cmd_tokens;
	char	*temp;
	char	*cmd_path;

	i = -1;
	directories = ft_split(get_env_values("PATH", env), ':');
	cmd_tokens = ft_split(cmd, ' ');
	while (directories[++i])
	{
		temp = ft_strjoin(directories[i], "/");
		cmd_path = ft_strjoin(temp, cmd_tokens[0]);
		free(temp);
		if (access(cmd_path, F_OK | X_OK) == 0)
		{
			free_arr(directories);
			free_arr(cmd_tokens);
			return (cmd_path);
		}
		free(cmd_path);
	}
	free_arr(directories);
	free_arr(cmd_tokens);
	return (NULL);
}

void	call_cmd(char *cmd, char *env[])
{
	char	**cmd_args;
	char	*cmd_path;

	cmd_args = ft_split(cmd, ' ');
	cmd_path = get_cmd_path(cmd_args[0], env);
	if (!cmd_path)
	{
		ft_putstr_fd("pipex: command not found: ", STDERR_FILENO);
		ft_putstr_fd(cmd_args[0], STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		free_arr(cmd_args);
		exit(EXIT_FAILURE);
	}
	if (execve(cmd_path, cmd_args, env) == -1)
	{
		free(cmd_path);
		free_arr(cmd_args);
		exit(EXIT_FAILURE);
	}
	free(cmd_path);
	free_arr(cmd_args);
}
