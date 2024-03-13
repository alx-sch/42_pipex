/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aschenk <aschenk@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 20:26:03 by aschenk           #+#    #+#             */
/*   Updated: 2024/03/13 22:35:49 by aschenk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// This file contains a utility functions for error messaging and exiting.

#include "pipex.h"
#include "libft/libft.h"

// FILE
void	perror_and_exit(char *msg, int *pipe_ends);

//	+++++++++++++++
//	++ FUNCTIONS ++
//	+++++++++++++++

// Displays an error message using perror() and exits with failure as status.
void	perror_and_exit(char *msg, int *pipe_ends)
{
	perror(msg);
	if (pipe_ends)
	{
		if (close(pipe_ends[0]) == -1)
			perror("Error closing pipe end [0]");
		if (close(pipe_ends[1]) == -1)
			perror("Error closing pipe end [1]");
	}
	exit(EXIT_FAILURE);
}
