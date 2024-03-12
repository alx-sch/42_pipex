/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aschenk <aschenk@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 19:16:26 by aschenk           #+#    #+#             */
/*   Updated: 2024/03/12 17:24:18 by aschenk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "libft/libft.h"

// FILE
void	child_process(char **argv, char **env, const int *pipe_ends);
void	parent_process(char **argv, char **env, const int *pipe_ends);

// call_cmd.c
void	call_cmd(char *cmd, char *env[]);

// utils.c
void	perror_and_exit(char *msg);

// libft
void	ft_putstr_fd(char *s, int fd);

//	+++++++++++++++
//	++ FUNCTIONS ++
//	+++++++++++++++

// close read end of pipe as not needed
// replace stdin fd with infile_fd
// redirect stdout fd with write end of pipe
// erro as reason can vary (does not exist, permission denied)
void	child_process(char **argv, char **env, const int *pipe_ends)
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

/// PARENT
void	parent_process(char **argv, char **env, const int *pipe_ends)
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
