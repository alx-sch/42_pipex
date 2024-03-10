/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aschenk <aschenk@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 18:45:47 by aschenk           #+#    #+#             */
/*   Updated: 2024/03/10 22:31:17 by aschenk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "libft/libft.h"

// processes.c
void	child_process(char **argv, char **env, const int *pipe_ends);
void	parent_process(char **argv, char **env, const int *pipe_ends);
void	handle_child_termination(int process_status);

// utils.c
void	errno_and_exit(const char *pre, const char *post);

//	+++++++++++++
//	++ PROGRAM ++
//	+++++++++++++

// child takes care of the left half of pipe (read)
// parent takes care of the right half of pipe (write)
int	main(int argc, char **argv, char **env)
{
	int		pipe_ends[2];
	pid_t	process_id;
	int		process_status;

	if (argc != 5)
	{
		ft_putstr_fd("Usage: ./pipex file1 cmd1 cmd2 file2\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	pipe(pipe_ends);
	process_id = fork();
	if (process_id == 0)
		child_process(argv, env, pipe_ends);
	else
	{
		waitpid(process_id, &process_status, 0);
		handle_child_termination(process_status);
		parent_process(argv, env, pipe_ends);
	}
	return (0);
}
