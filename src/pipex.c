/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aschenk <aschenk@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 18:45:47 by aschenk           #+#    #+#             */
/*   Updated: 2024/03/07 13:04:35 by aschenk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "libft/libft.h"

// process.c
void	child_process(char *argv[], char *env[], int *pipe_fd);

// utils.c
//void	perror_and_exit(char *message);

//	+++++++++++++
//	++ PROGRAM ++
//	+++++++++++++

// child takes care of the left half of pipe (read)
// parent takes care of the right half of pipe (write)
int	main(int argc, char *argv[], char *env[])
{
	int		pipe_fd[2];
	int		process_id;
	char	*cat_args[3];
	char	*grep_args[3];

	if (argc != 5)
	{
		ft_putstr_fd("ERROR: ./pipex file1 cmd1 cmd2 file2\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	if (pipe(pipe_fd) == -1)
		perror_and_exit("pipex");
	process_id = fork();
	if (process_id == -1)
		perror_and_exit("fork");
	if (process_id == 0)
		child_process(argv, env, pipe_fd);
	parent_process(argv, env, pipe_fd);
	return (0);
}



// 	(void) argc;
// 	(void) argv;
// 	cat_args[0] = "/bin/cat";
// 	cat_args[1] = "test.txt";
// 	cat_args[2] = NULL;
// 	grep_args[0] = "/bin/grep";
// 	grep_args[1] = "Lausanne";
// 	grep_args[2] = NULL;

// 	if (pipe(pipe_fd) == -1)
// 	{
// 		perror("pipe");
// 		exit(EXIT_FAILURE);
// 	}
// 	p_id = fork();
// 	if (p_id == -1)
// 	{
// 		perror("fork");
// 		exit(EXIT_FAILURE);
// 	}
// 	else if (p_id ==0)
// 	{
// 		close(pipe_fd[1]);
// 		dup2(pipe_fd[0], STDIN_FILENO);
// 		execve("/bin/grep", grep_args, env);
// 	}
// 	else
// 	{
// 		close(pipe_fd[0]);
// 		dup2(pipe_fd[1], STDOUT_FILENO);
// 		execve("/bin/cat", cat_args, env);
// 	}
// 	close(pipe_fd[0]);
// 	close(pipe_fd[1]);
// 	waitpid(p_id, NULL, 0);
// 	return (0);
// }
