/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aschenk <aschenk@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 18:45:47 by aschenk           #+#    #+#             */
/*   Updated: 2024/03/13 12:41:14 by aschenk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// This file implements the main() for the pipex program.

#include "pipex.h"
#include "libft/libft.h"

// FILE
int		main(int argc, char **argv, char **env);

// pipeline.c
void	pipeline_left(char **argv, char **env, const int *pipe_ends);
void	pipeline_right(char **argv, char **env, const int *pipe_ends);

// utils.c
void	perror_and_exit(char *msg);

//	+++++++++++++++
//	++ FUNCTIONS ++
//	+++++++++++++++

// Checks for the correct number of CL arguments and if the last one (outfile)
// can be accessed. If the outfile does not exist, it creates it with
// 'rw-r--r--' permissions.
static void	check_args(int argc, char **argv)
{
	if (argc != 5)
	{
		ft_putstr_fd("Usage: ./pipex file1 cmd1 cmd2 file2\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	if (access(argv[4], F_OK) == -1)
	{
		if (open(argv[4], O_WRONLY | O_CREAT, 0644) == -1)
			perror_and_exit("open");
	}
}

//	+++++++++++++
//	++ PROGRAM ++
//	+++++++++++++

// Handles the creation of a pipeline, the forking process and executing
// commands within the pipeline in the correct order. The left half of the
// pipeline is handled by the child process, while the right half is handled
// by the parent process.
int	main(int argc, char **argv, char **env)
{
	int		pipe_ends[2];
	pid_t	process_id;

	check_args(argc, argv);
	if (pipe(pipe_ends) == -1)
		perror_and_exit("pipe");
	process_id = fork();
	if (process_id == -1)
		perror_and_exit("pipe");
	if (process_id == 0)
		pipeline_left(argv, env, pipe_ends);
	else
	{
		waitpid(process_id, NULL, 0);
		pipeline_right(argv, env, pipe_ends);
	}
}
