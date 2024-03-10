/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aschenk <aschenk@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 19:16:26 by aschenk           #+#    #+#             */
/*   Updated: 2024/03/10 22:30:31 by aschenk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "libft/libft.h"

// FILE
void	child_process(char **argv, char **env, const int *pipe_ends);
void	parent_process(char **argv, char **env, const int *pipe_ends);
void	handle_child_termination(int process_status);

// call_cmd.c
void	call_cmd(char *cmd, char *env[]);

// libft
void	ft_putstr_fd(char *s, int fd);

// 0: inout
// 1: output
// defined in header
static int	open_file(char *file, int access_mode)
{
	int	_return;

	_return = -1;
	if (access_mode == READ_MODE)
		_return = open(file, O_RDONLY);
	else if (access_mode == WRITE_MODE)
		_return = open(file, O_WRONLY | O_TRUNC | O_CREAT, 0777);
	return (_return);
}

// close read end of pipe as not needed
// replace stdin fd with infile_fd
// redirect stdout fd with write end of pipe
// erro as reason can vary (does not exist, permission denied)
void	child_process(char **argv, char **env, const int *pipe_ends)
{
	int	infile_fd;

	infile_fd = open_file(argv[1], READ_MODE);
	if (infile_fd == -1)
	{
		ft_putstr_fd("pipex: ", STDERR_FILENO);
		ft_putstr_fd(strerror(errno), STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(argv[1], STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	close(pipe_ends[0]);
	dup2(infile_fd, STDIN_FILENO);
	close(infile_fd);
	dup2(pipe_ends[1], STDOUT_FILENO);
	call_cmd(argv[2], env);
}

void	parent_process(char **argv, char **env, const int *pipe_ends)
{
	int	outfile_fd;

	outfile_fd = open_file(argv[4], WRITE_MODE);
	if (outfile_fd == -1)
	{
		ft_putstr_fd("pipex: ", STDERR_FILENO);
		ft_putstr_fd(strerror(errno), STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(argv[4], STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	close(pipe_ends[1]);
	dup2(outfile_fd, STDOUT_FILENO);
	close(outfile_fd);
	dup2(pipe_ends[0], STDIN_FILENO);
	call_cmd(argv[3], env);
}

void	handle_child_termination(int process_status)
{
	if (WIFEXITED(process_status))
	{
		if (WEXITSTATUS(process_status) == EXIT_FAILURE)
			exit(EXIT_FAILURE);
	}
	else
	{
		ft_putstr_fd("Child process terminated abnormally\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
}
