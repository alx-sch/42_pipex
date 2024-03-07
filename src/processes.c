/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aschenk <aschenk@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 19:16:26 by aschenk           #+#    #+#             */
/*   Updated: 2024/03/07 20:04:04 by aschenk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "libft/libft.h"

// utils.c
void	perror_and_exit(char *message);

// libft
char	**ft_split(const char *s, char c);

// 0: inout
// 1: output
// defined in header
static int	open_file(char *file, int access_mode)
{
	int	_return;

	if (access_mode == READ_MODE)
		_return = open(file, O_RDONLY);
	else if (access_mode == WRITE_MODE)
		_return = open(file, O_WRONLY | O_TRUNC | O_CREAT, 0777);
	else
	{
		ft_putstr_fd("ERROR: Invalid access mode\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	return (_return);
}

// char	*get_cmd_path(char *cmd, char **env)
// {
// 	int		i;
// 	char	*exec;
// 	char	*allpath[];
// 	char	*path_part;
// 	char	*s_cmd[];

// 	i = -1;
// 	allpath = ft_split(my_getenv("PATH", env), ':');
// 	s_cmd = ft_split(cmd, ' ');
// 	while (allpath[++i])
// 	{
// 		path_part = ft_strjoin(allpath[i], "/");
// 		exec = ft_strjoin(path_part, s_cmd[0]);
// 		free(path_part);
// 		if (access(exec, F_OK | X_OK) == 0)
// 		{
// 			ft_free_tab(s_cmd);
// 			return (exec);
// 		}
// 		free(exec);
// 	}
// 	ft_free_tab(allpath);
// 	ft_free_tab(s_cmd);
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

// close read end of pipe as not needed
// replace stdin fd with infile_fd
// redirect stdout fd with write end of pipe
void	child_process(char **argv, char **env, const int *pipe_fd)
{
	int	infile_fd;

	(void)env;
	infile_fd = open_file(argv[1], READ_MODE);
	if (infile_fd == -1)
		perror_and_exit("open");
	if (close(pipe_fd[0]) == -1)
		perror_and_exit("close");
	if (dup2(infile_fd, STDIN_FILENO) == -1)
		perror_and_exit("dup2");
	if (close(infile_fd) == -1)
		perror_and_exit("dup2");
	//execute(argv[2], env);
}

// void	parent_process(char *argv[], char *env[], int *pipe_fd)
// {
// 	int	outfile_fd;

// 	outfile_fd = open_file(argv[4], WRITE_MODE);
// 	if (outfile_fd == -1)
// 		perror_and_exit("open");
// 	if (close(p_fd[1]) == -1)
// 		perror_and_exit("close");
// 	if (dup2(outfile_fd, STDOUT_FILENO) == -1)
// 		perror_and_exit("dup2");
// 	execute(argv[3], env);
// }
