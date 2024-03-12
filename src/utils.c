/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aschenk <aschenk@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 20:26:03 by aschenk           #+#    #+#             */
/*   Updated: 2024/03/12 17:08:34 by aschenk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "libft/libft.h"

// FILE
void	perror_and_exit(char *msg);
void	check_args(int argc, char **argv);

//	+++++++++++++++
//	++ FUNCTIONS ++
//	+++++++++++++++

// Displays an error message using perror() and exits with failure as status.
void	perror_and_exit(char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

// Checks for the correct number of CL arguments and if the last one (outfile)
// can be accessed. If the outfile does not exist, it creates it with
// 'rw-r--r--' permissions.
void	check_args(int argc, char **argv)
{
	if (argc != 5)
	{
		ft_putstr_fd("Usage: ./pipex file1 cmd1 cmd2 file2\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	if (access(argv[4], F_OK) == -1)
	{
		if (open(argv[4], O_WRONLY | O_CREAT, 0644) == -1)
			perror_and_exit("open");
	}
}
