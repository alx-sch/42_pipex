/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aschenk <aschenk@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 18:45:47 by aschenk           #+#    #+#             */
/*   Updated: 2024/03/12 17:16:12 by aschenk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "libft/libft.h"

// processes.c
void	child_process(char **argv, char **env, const int *pipe_ends);
void	parent_process(char **argv, char **env, const int *pipe_ends);

// utils.c
void	check_args(int argc, char **argv);
void	perror_and_exit(char *msg);

//	+++++++++++++
//	++ PROGRAM ++
//	+++++++++++++

// child takes care of the left half of pipe (read)
// parent takes care of the right half of pipe (write)
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
		child_process(argv, env, pipe_ends);
	else
	{
		waitpid(process_id, NULL, 0);
		parent_process(argv, env, pipe_ends);
	}
	return (0);
}
