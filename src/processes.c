/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aschenk <aschenk@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 19:16:26 by aschenk           #+#    #+#             */
/*   Updated: 2024/03/10 21:23:41 by aschenk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "libft/libft.h"

// FILE
void	child_process(char **argv, char **env, const int *pipe_fd);
void	handle_child_termination(int process_status);

// call_cmd.c
void	call_cmd(char *cmd, char *env[]);

// utils.c
void	msg_and_exit(char *msg);
void	errno_and_exit(const char *pre, const char *post);
void	free_arr(char **array);

// libft
char	**ft_split(const char *s, char c);

// 0: inout
// 1: output
// defined in header
static int	open_file(char *file, int access_mode)
{
	int	_return;

	_return = -1;
	if (access_mode == READ_MODE)
		_return = open(file, O_RDONLY);
	else if (access_mode == WRITE_MODE)
		_return = open(file, O_WRONLY | O_TRUNC | O_CREAT, 0777);
	return (_return);
}

// void	execute(char *cmd, char *env[])
// {
// 	char	*cmd_args[];
// 	char	*cmd_path;

// 	cmd_args = ft_split(cmd, ' ');
// 	cmd_path = get_cmd_path(cmd_args[0], env);
// 	if (execve(path, cmd_args, env) == -1)
// 	{
// 		ft_putstr_fd("pipex: command not found: ", 2);
// 		ft_putendl_fd(cmd_tokens[0], 2);
// 		ft_free_tab(cmd_tokens);
// 		exit(0);
// 	}
// }

// close read end of pipe as not needed
// replace stdin fd with infile_fd
// redirect stdout fd with write end of pipe
// erro as reason can vary (does not exist, permission denied)
void	child_process(char **argv, char **env, const int *pipe_fd)
{
	int	infile_fd;

	infile_fd = open_file(argv[1], READ_MODE);
	if (infile_fd == -1)
	{
		ft_putstr_fd("pipex: ", STDERR_FILENO);
		ft_putstr_fd(strerror(errno), STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(argv[1], STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
		//errno_and_exit("pipex: ", argv[1]);
	close(pipe_fd[0]);
	dup2(infile_fd, STDIN_FILENO);
	close(infile_fd);
	call_cmd(argv[2], env);
}

void	handle_child_termination(int process_status)
{
	if (WIFEXITED(process_status))
	{
		if (WEXITSTATUS(process_status) == EXIT_FAILURE)
			exit(EXIT_FAILURE);
	}
	else
	{
		ft_putstr_fd("Child process terminated abnormally\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
}

// void	parent_process(char *argv[], char *env[], int *pipe_fd)
// {
// 	int	outfile_fd;

// 	outfile_fd = open_file(argv[4], WRITE_MODE);
// 	if (outfile_fd == -1)
// 		perror_and_exit("open");
// 	if (close(p_fd[1]) == -1)
// 		perror_and_exit("close");
// 	if (dup2(outfile_fd, STDOUT_FILENO) == -1)
// 		perror_and_exit("dup2");
// 	execute(argv[3], env);
// }
