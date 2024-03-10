/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aschenk <aschenk@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 16:23:24 by aschenk           #+#    #+#             */
/*   Updated: 2024/03/10 16:50:01 by aschenk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void	child_process(int id, int *pipe_fds)
{
	close(pipe_fds[0]); // Close unusd read end
	printf("Child %d starting...\n", id);
	sleep(id);
	printf("Child %d finished its work\n", id);
	write(pipe_fds[1], &id, sizeof(id)); // Write to the pipe
	close(pipe_fds[1]); // Close write after writing
	exit(0);
}

int	main(void)
{
	int		pipe_fds[2];
	pid_t	process_id;
	int		i;
	int		num_children;
	pid_t	child_id;

	num_children = 4;
	if (pipe(pipe_fds) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	i = 1;
	while (i < num_children + 1)
	{
		process_id = fork();
		if (process_id == -1)
		{
			perror("pipe");
			exit(EXIT_FAILURE);
		}
		else if (process_id == 0)
			child_process(i, pipe_fds);
		else
			i++;
	}

	// Parent process
	i = 0;
	printf("Parent waiting for children to finish...\n");
	while (i < num_children)
	{
		wait(NULL);
		if (read(pipe_fds[0], &child_id, sizeof(child_id)) == -1)
		{
			perror("read");
			exit(EXIT_FAILURE);
		}
		printf("Parent received notification from Child %d\n", child_id);
		i++;
	}

	printf("Parent continuing...\n");

	close(pipe_fds[0]); // Close read end of the pipe

	return (0);
}
