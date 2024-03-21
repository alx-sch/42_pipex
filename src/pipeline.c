/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aschenk <aschenk@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 19:16:26 by aschenk           #+#    #+#             */
/*   Updated: 2024/03/21 15:56:20 by aschenk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// This file contains the implementation of two functions, responsible for
// executing the left and right side of the pipeline. As well as a function
// executed within the parent process monitoring the exit status of the last
// (right) child process.

#include "pipex.h"
#include "libft/libft.h"

// FILE
void	pipeline_left(char **argv, char **env, int *pipe_ends);
void	pipeline_right(char **argv, char **env, int *pipe_ends);
void	parent_process(int process_id, int *pipe_ends);

// call_cmd.c
void	call_cmd(char *cmd, char **env);

// utils.c
void	perror_and_exit(char *msg, int *pipe_ends);

// libft
void	ft_putstr_fd(char *s, int fd);
char	*ft_strdup(const char *src);
int		ft_tolower(int c);

//	+++++++++++++++
//	++ FUNCTIONS ++
//	+++++++++++++++

// Converts a string to lowercase.
static void	str_tolower(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		str[i] = ft_tolower(str[i]);
		i++;
	}
}

// Prints an error message related to file operations, e.g. open().
// Makes sure that the message obtained from strerror(errno) is all lowercase
// to imitate the shell's behavior (zsh).
static void	print_file_err_msg(char *pre, char *file, int *pipe_ends)
{
	char	*errno_message;

	errno_message = ft_strdup(strerror(errno));
	if (!errno_message)
		perror_and_exit("malloc", pipe_ends);
	str_tolower(errno_message);
	ft_putstr_fd(pre, STDERR_FILENO);
	ft_putstr_fd(errno_message, STDERR_FILENO);
	free(errno_message);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(file, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

// Executes the left side of the process (child process).
// - Opens the input file ('infile') specified by the first CL argument.
// - Closes the unused read end of the pipe.
// - Redirects standard input to read from the input file ('< infile' in CL).
// - Redirects standard output to write to the write end of the pipe,
//   which will serve as the input for the right side of the pipeline.
// - Executes the command (second CL argument) using 'call_cmd'/'execve'.
// - The process terminates automatically after the command execution,
//	 either due to success or failure of execve().
void	pipeline_left(char **argv, char **env, int *pipe_ends)
{
	int		infile_fd;

	infile_fd = open(argv[1], O_RDONLY);
	if (close(pipe_ends[0]) == -1)
		perror_and_exit("close", pipe_ends);
	if (infile_fd == -1)
	{
		print_file_err_msg("pipex: ", argv[1], pipe_ends);
		if (close(pipe_ends[1]) == -1)
			perror_and_exit("close", pipe_ends);
		exit(EXIT_FAILURE);
	}
	if (dup2(infile_fd, STDIN_FILENO) == -1)
		perror_and_exit("dup2", pipe_ends);
	if (close(infile_fd) == -1)
		perror_and_exit("close", pipe_ends);
	if (dup2(pipe_ends[1], STDOUT_FILENO) == -1)
		perror_and_exit("dup2", pipe_ends);
	if (close(pipe_ends[1]) == -1)
		perror_and_exit("close", pipe_ends);
	call_cmd(argv[2], env);
}

// Executes the right side of the process (child of child process).
// - Opens the output file ('outfile') specified by the fourth CL argument.
// 	 If the outfile does not exist, it creates it with 'rw-r--r--' permissions.
// - Closes the unused write end of the pipe.
// - Redirects standard input to read from the read end of the pipe,
//   which receives input from the previous process in the pipeline.
// - Redirects standard output to write to the output file ('> outfile' in CL).
// - Executes the command (third CL argument) using 'call_cmd'/'execve'.
// - The process terminates automatically after the command execution,
//	 either due to success or failure of execve().
void	pipeline_right(char **argv, char **env, int *pipe_ends)
{
	int		outfile_fd;

	outfile_fd = open(argv[4], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (close(pipe_ends[1]) == -1)
		perror_and_exit("close", pipe_ends);
	if (outfile_fd == -1)
	{
		print_file_err_msg("pipex: ", argv[4], pipe_ends);
		if (close(pipe_ends[0]) == -1)
			perror_and_exit("close", pipe_ends);
		exit(EXIT_FAILURE);
	}
	if (dup2(pipe_ends[0], STDIN_FILENO) == -1)
		perror_and_exit("dup2", pipe_ends);
	if (close(pipe_ends[0]) == -1)
		perror_and_exit("close", pipe_ends);
	if (dup2(outfile_fd, STDOUT_FILENO) == -1)
		perror_and_exit("dup2", pipe_ends);
	if (close(outfile_fd) == -1)
		perror_and_exit("close", pipe_ends);
	call_cmd(argv[3], env);
}

// Waits for a specified process to finish and forwards the process' exit status
// to the parent.
void	parent_process(int process_id, int *pipe_ends)
{
	int	child_status;

	if (close(pipe_ends[0]) == -1)
		perror_and_exit("close", pipe_ends);
	if (close(pipe_ends[1]) == -1)
		perror_and_exit("close", pipe_ends);
	waitpid(process_id, &child_status, 0);
	if (WIFEXITED(child_status) && WEXITSTATUS(child_status) != EXIT_SUCCESS)
		exit(EXIT_FAILURE);
	else
		exit(EXIT_SUCCESS);
}
