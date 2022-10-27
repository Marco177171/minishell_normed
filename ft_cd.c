/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masebast <masebast@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/06 17:19:47 by gmeoli            #+#    #+#             */
/*   Updated: 2022/10/25 18:50:43 by masebast         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_go_to_parent(char *path)
{
	int		index;
	char	*destination;
	int		slash_count;
	int		slashes;

	index = 0;
	slash_count = 0;
	slashes = 0;
	while (path[index])
	{
		if (path[index] == '/')
			slash_count++;
		index++;
	}
	index = 0;
	destination = malloc(sizeof(char) * ft_find_dest_len(path));
	while (path[index] && slashes < slash_count)
	{
		if (slashes + 1 == slash_count && path[index] == '/')
			break ;
		destination[index] = path[index];
		if (path[index] == '/')
			slashes++;
		index++;
	}
	destination[index] = '\0';
	return (destination);
}

char	*ft_go_home(char *path)
{
	int		index;
	char	*destination;
	int		slashes;

	index = 0;
	slashes = 0;
	destination = malloc(sizeof(char) * ft_find_home_len(path));
	while (path[index] && slashes <= 2)
	{
		if (slashes == 2 && path[index] == '/')
			break ;
		destination[index] = path[index];
		if (path[index] == '/')
			slashes++;
		index++;
	}
	destination[index] = '\0';
	return (destination);
}

int	ft_cd(t_command *command_struct)
{
	char	path[PATH_MAX];
	char	*ret;

	*g_exit_status = 0;
	getcwd(path, sizeof(path));
	if (!command_struct->word_matrix[1])
	{
		ret = ft_go_home(path);
		chdir(ret);
		free(ret);
		return (*g_exit_status);
	}
	else if (ft_strcmp(command_struct->word_matrix[1], "..") == 0)
	{
		ret = ft_go_to_parent(path);
		chdir(ret);
		free(ret);
		return (*g_exit_status);
	}
	else
	{
		chdir(command_struct->word_matrix[1]);
		return (*g_exit_status);
	}
}
