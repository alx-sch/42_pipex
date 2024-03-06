/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aschenk <aschenk@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 18:44:40 by aschenk           #+#    #+#             */
/*   Updated: 2024/03/06 20:15:57 by aschenk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "libft/libft.h"

// libft
void	ft_putchar_fd(char c, int fd);

//	+++++++++++++++
//	++ FUNCTIONS ++
//	+++++++++++++++

void	perror_and_exit(char *message)
{
	perror(message, STDERR_FILENO);
	exit(EXIT_FAILURE);
}

// 0: inout
// 1: output
int	open_file(char *file, int access_mode)
{
	int	_return;

	if (access_mode == 0)
		_return = open(file, O_RDONLY);
	if (access_mode == 1)
		_return = open(file, O_WRONLY | O_TRUNC | O_CREAT, 0777);
	if (_return == -1)
	{
		perror("Open failure");
		exit(EXIT_FAILURE);
	}
	return (_return);
}
