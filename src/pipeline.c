/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aschenk <aschenk@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 19:16:26 by aschenk           #+#    #+#             */
/*   Updated: 2024/03/13 12:41:00 by aschenk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// This file contains the implementation of two functions, responsible for
// executing the left and right side of the pipeline.

#include "pipex.h"
#include "libft/libft.h"

// FILE
void	pipeline_left(char **argv, char **env, const int *pipe_ends);
void	pipeline_right(char **argv, char **env, const int *pipe_ends);

// call_cmd.c
void	call_cmd(char *cmd, char *env[]);

// utils.c
void	perror_and_exit(char *msg);

// libft
void	ft_putstr_fd(char *s, int fd);

//	+++++++++++++++
//	++ FUNCTIONS ++
//	+++++++++++++++

// Executes the left side of the process (in the child process).
// - Opens the input file ('infile') specified by the first CL argument.
// - Closes the unused read end of the pipe.
// - Redirects standard input to read from the input file ('< infile' in CL).
// - Redirects standard output to write to the write end of the pipe,
//   which will serve as the input for the right side of the pipeline.
// - Executes the command (second CL argument) using 'call_cmd'/'execve'.
// - The child process terminates automatically after the command execution,
//	 either due to success or failure of execve().
void	pipeline_left(char **argv, char **env, const int *pipe_ends)
{
	int	infile_fd;

	infile_fd = open(argv[1], O_RDONLY);
	if (infile_fd == -1)
	{
		ft_putstr_fd("pipex: ", STDERR_FILENO);
		ft_putstr_fd(strerror(errno), STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(argv[1], STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	if (close(pipe_ends[0]) == -1)
		perror_and_exit("close");
	if (dup2(infile_fd, STDIN_FILENO) == -1)
		perror_and_exit("dup2");
	if (close(infile_fd) == -1)
		perror_and_exit("close");
	if (dup2(pipe_ends[1], STDOUT_FILENO) == -1)
		perror_and_exit("dup2");
	if (close(pipe_ends[1]) == -1)
		perror_and_exit("close");
	call_cmd(argv[2], env);
}

// Executes the right side of the process (in the parent process).
// - Opens the output file ('outfile') specified by the fourth CL argument.
// - Closes the unused write end of the pipe.
// - Redirects standard input to read from the read end of the pipe,
//   which receives input from the previous process in the pipeline.
// - Redirects standard output to write to the output file ('> outfile' in CL).
// - Executes the command (third CL argument) using 'call_cmd'/'execve'.
// - The parent process terminates automatically after the command execution,
//	 either due to success or failure of execve().
void	pipeline_right(char **argv, char **env, const int *pipe_ends)
{
	int	outfile_fd;

	outfile_fd = open(argv[4], O_WRONLY | O_TRUNC);
	if (outfile_fd == -1)
	{
		ft_putstr_fd("pipex: ", STDERR_FILENO);
		ft_putstr_fd(strerror(errno), STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(argv[4], STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	if (close(pipe_ends[1]) == -1)
		perror_and_exit("close");
	if (dup2(outfile_fd, STDOUT_FILENO) == -1)
		perror_and_exit("dup2");
	if (close(outfile_fd) == -1)
		perror_and_exit("close");
	if (dup2(pipe_ends[0], STDIN_FILENO) == -1)
		perror_and_exit("dup2");
	if (close(pipe_ends[0]) == -1)
		perror_and_exit("close");
	call_cmd(argv[3], env);
}
