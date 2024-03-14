/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aschenk <aschenk@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 18:45:47 by aschenk           #+#    #+#             */
/*   Updated: 2024/03/14 21:38:16 by aschenk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// This file implements the main() for the pipex program.

#include "pipex.h"
#include "libft/libft.h"

// FILE
int		main(int argc, char **argv, char **env);

// pipeline.c
void	pipeline_left(char **argv, char **env, int *pipe_ends);
void	pipeline_right(char **argv, char **env, int *pipe_ends);
void	parent_process(int process_id, int *pipe_ends);

// utils.c
void	perror_and_exit(char *msg, int *pipe_ends);

//	+++++++++++++++
//	++ FUNCTIONS ++
//	+++++++++++++++

// Verifies the correct number of CL arguments provided to the program.
static void	check_args(int argc)
{
	if (argc != 5)
	{
		ft_putstr_fd("Usage: ./pipex file1 cmd1 cmd2 file2\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
}

//	+++++++++++++
//	++ PROGRAM ++
//	+++++++++++++


// Manages the creation of a pipeline, forking processes, and executing
// commands within the pipeline. Each half of the pipeline (left + right)
// is handled by a child process, while the parent process ensures to
// propagate the exit status of the right side of the pipe.
int	main(int argc, char **argv, char **env)
{
	int		pipe_ends[2];
	pid_t	process_id_1;
	pid_t	process_id_2;

	check_args(argc);
	if (pipe(pipe_ends) == -1)
		perror_and_exit("pipe", NULL);
	process_id_1 = fork();
	if (process_id_1 == -1)
		perror_and_exit("fork", pipe_ends);
	if (process_id_1 == 0)
	{
		process_id_2 = fork();
		if (process_id_2 == -1)
			perror_and_exit("fork", pipe_ends);
		if (process_id_2 == 0)
			pipeline_left(argv, env, pipe_ends);
		else
			pipeline_right(argv, env, pipe_ends);
	}
	else
		parent_process(process_id_1, pipe_ends);
}
