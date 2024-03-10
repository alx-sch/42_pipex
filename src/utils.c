/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aschenk <aschenk@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 18:44:40 by aschenk           #+#    #+#             */
/*   Updated: 2024/03/10 21:24:09 by aschenk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "libft/libft.h"

// FILE
//void	errno_and_exit(const char *pre, const char *post);
void	free_arr(char **array);

// libft
//void	ft_putstr_fd(char *s, int fd);
//char	*ft_strjoin(const char *s1, const char *s2);

//	+++++++++++++++
//	++ FUNCTIONS ++
//	+++++++++++++++

// char	*add_newline(const char *str)
// {
// 	char	*nl_added;

// 	nl_added = ft_strjoin(str, "\n");
// 	if (!nl_added)
// 		exit(EXIT_FAILURE);
// 	return (nl_added);
// }

// static char	*process_msg(const char *pre, const char *post)
// {
// 	char	*colon_added;
// 	char	*post_added;
// 	char	*nl_added;

// 	colon_added = ft_strjoin(pre, ": ");
// 	if (!colon_added)
// 		exit(EXIT_FAILURE);
// 	post_added = ft_strjoin(colon_added, post);
// 	free(colon_added);
// 	if (!post_added)
// 		exit(EXIT_FAILURE);
// 	nl_added = ft_strjoin(post_added, "\n");
// 	free(post_added);
// 	if (!nl_added)
// 		exit(EXIT_FAILURE);
// 	return (nl_added);
// }

// void	errno_and_exit(const char *pre, const char *post)
// {
// 	char	*error_message;
// 	char	*error_message_nl;

// 	error_message = ft_strjoin(pre, strerror(errno));
// 	if (!error_message)
// 		exit(EXIT_FAILURE);
// 	if (post == NULL)
// 	{
// 		error_message_nl = ft_strjoin(error_message, "\n");
// 		if (!error_message_nl)
// 		{
// 			free(error_message);
// 			exit(EXIT_FAILURE);
// 		}
// 		ft_putstr_fd(error_message_nl, STDERR_FILENO);
// 		free(error_message_nl);
// 	}
// 	else
// 	{
// 		error_message_nl = process_msg(error_message, post);
// 		ft_putstr_fd(error_message_nl, STDERR_FILENO);
// 		free(error_message_nl);
// 	}
// 	exit(EXIT_FAILURE);
// }

void	free_arr(char **array)
{
	size_t	i;

	i = 0;
	if (!array)
		return ;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}
