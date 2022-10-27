/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_utility.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masebast <masebast@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 18:49:43 by masebast          #+#    #+#             */
/*   Updated: 2022/10/25 18:51:21 by masebast         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_find_dest_len(char *path)
{
	int	index;
	int	total_slashes;
	int	slashes;

	index = 0;
	total_slashes = 0;
	slashes = 0;
	while (path[index])
	{
		if (path[index] == '/')
			total_slashes++;
		index++;
	}
	index = 0;
	while (path[index] && slashes < total_slashes)
	{
		if (path[index] == '/')
			slashes++;
		index++;
	}
	return (index);
}

int	ft_find_home_len(char *path)
{
	int	index;
	int	slashes;

	index = 0;
	slashes = 0;
	while (path[index] && slashes <= 2)
	{
		if (path[index] == '/')
			slashes++;
		index++;
	}
	return (index);
}
