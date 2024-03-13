/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aschenk <aschenk@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 20:26:03 by aschenk           #+#    #+#             */
/*   Updated: 2024/03/13 12:40:34 by aschenk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// This file contains a utility functions for error messaging and exiting.

#include "pipex.h"
#include "libft/libft.h"

// FILE
void	perror_and_exit(char *msg);

//	+++++++++++++++
//	++ FUNCTIONS ++
//	+++++++++++++++

// Displays an error message using perror() and exits with failure as status.
void	perror_and_exit(char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}
