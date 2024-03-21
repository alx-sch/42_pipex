/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aschenk <aschenk@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 19:43:13 by aschenk           #+#    #+#             */
/*   Updated: 2024/03/21 17:27:24 by aschenk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdlib.h> // malloc, free, exit, access, dup2, execv
# include <fcntl.h> // open
# include <unistd.h> // fork, pipe, macros like 'STDERR_FILENO'
# include <stdio.h> // perror
# include <string.h> // strerror
# include <errno.h> // errno
# include <sys/wait.h> // waitpid

//	+++++++++++++
//	++ PROGRAM ++
//	+++++++++++++

// main.c
int		main(int argc, char *argv[], char *env[]);

//	+++++++++++++++
//	++ FUNCTIONS ++
//	+++++++++++++++

// pipeline.c
void	pipeline_left(char **argv, char **env, int *pipe_ends);
void	pipeline_right(char **argv, char **env, int *pipe_ends);
void	parent_process(int process_id_1, int process_id_2, int *pipe_ends);

// call_cmd.c
void	call_cmd(char *cmd, char **env);

// utils.c
void	perror_and_exit(char *msg, int *pipe_ends);

#endif
