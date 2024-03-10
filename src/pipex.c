/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aschenk <aschenk@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 18:45:47 by aschenk           #+#    #+#             */
/*   Updated: 2024/03/10 20:36:29 by aschenk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "libft/libft.h"

// process.c
int		child_process(char *argv[], char *env[], const int *pipe_ends);

// utils.c
void	msg_and_exit(char *msg);
void	errno_and_exit(const char *pre, const char *post);
void	handle_child_termination(int process_status);

// libft

char	*get_cmd_path(char *cmd, char **env);

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
		//parent_process()
	}
}



// 	(void) argc;
// 	(void) argv;
// 	cat_args[0] = "/bin/cat";
// 	cat_args[1] = "test.txt";
// 	cat_args[2] = NULL;
// 	grep_args[0] = "/bin/grep";
// 	grep_args[1] = "Lausanne";
// 	grep_args[2] = NULL;

// 	if (pipe(pipe_ends) == -1)
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
// 		close(pipe_ends[1]);
// 		dup2(pipe_ends[0], STDIN_FILENO);
// 		execve("/bin/grep", grep_args, env);
// 	}
// 	else
// 	{
// 		close(pipe_ends[0]);
// 		dup2(pipe_ends[1], STDOUT_FILENO);
// 		execve("/bin/cat", cat_args, env);
// 	}
// 	close(pipe_ends[0]);
// 	close(pipe_ends[1]);
// 	waitpid(p_id, NULL, 0);
// 	return (0);
// }
