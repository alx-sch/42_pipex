/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aschenk <aschenk@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 19:43:13 by aschenk           #+#    #+#             */
/*   Updated: 2024/03/10 19:25:50 by aschenk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdlib.h> // malloc, free, exit, access, dup, dup2, execv
# include <fcntl.h> // open
# include <unistd.h> // fork, pipe, unlink, macros like STDERR_FILENO
# include <stdio.h> // perror
# include <string.h> // strerror
# include <sys/wait.h> // wait, waitpid
# include <errno.h> // errno

// Macros for open_file()
# define READ_MODE 0
# define WRITE_MODE 1

typedef struct s_pipex
{
	int		infile;
	int		outfile;
	int		tube[2];
	int		pid1;
	int		pid2;
	char	**command;
	char	*command_path;
	char	**splitted_path;
	char	*env_path;
}	t_pipex;


//	+++++++++++++
//	++ PROGRAM ++
//	+++++++++++++

// pipex.c
int	main(int argc, char *argv[], char *env[]);

//	+++++++++++++++
//	++ FUNCTIONS ++
//	+++++++++++++++

#endif
