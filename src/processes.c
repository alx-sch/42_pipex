/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aschenk <aschenk@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 19:16:26 by aschenk           #+#    #+#             */
/*   Updated: 2024/03/06 20:33:59 by aschenk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "libft/libft.h"

// utils.c
void	perror_and_exit(char *message)

// libft
char	**ft_split(const char *s, char c);

// 0: inout
// 1: output
// defined in header
static int	open_file(char *file, int access_mode)
{
	int	_return;

	if (access_mode == READ_MODE)
		_return = open(file, O_RDONLY);
	if (access_mode == WRITE_MODE)
		_return = open(file, O_WRONLY | O_TRUNC | O_CREAT, 0777);
	return (_return);
}

void	execute(char *cmd, char *env[])
{
	char	*cmd_args[];
	char	*path;

	cmd_args = ft_split(cmd, ' ');
	path = get_path(cmd_args[0], env);
	if (execve(path, cmd_args, env) == -1)
	{
		ft_putstr_fd("pipex: command not found: ", 2);
		ft_putendl_fd(s_cmd[0], 2);
		ft_free_tab(s_cmd);
		exit(0);
	}
}

// close read end of pipe as not needed
// replace stdin fd with infile_fd
// redirect stdout fd with write end of pipe
void	child_process(char *argv[], char *env[], int *pipe_fd)
{
	int	infile_fd;

	infile_fd = open_file(argv[1], READ_MODE);
	if (infile_fd == -1)
		perror_and_exit("Failed to open input file");
	if (close(p_fd[0]) == -1)
		perror_and_exit("Failed to close the read end of pipe");
	if (dub2(infile_fd, STDIN_FILENO) == -1)
		perror_and_exit("Failed to redirect stdin to input file");
	execute(argv[2], env);
}

void	parent_process(char *argv[], char *env[], int *pipe_fd)
{
	int	outfile_fd;

	outfile_fd = open_file(argv[4], WRITE_MODE);
	if (outfile_fd == -1)
		perror_and_exit("Failed to open/create output file");
	if (close(p_fd[1]) == -1)
		perror_and_exit("Failed to close the write end of pipe");
	if (dub2(outfile_fd, STDOUT_FILENO) == -1)
		perror_and_exit("Failed to redirect stdout to output file");
	execute(argv[3], env);
}
