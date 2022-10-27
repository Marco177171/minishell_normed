/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo_utility.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmeoli <gmeoli@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/20 14:50:36 by masebast          #+#    #+#             */
/*   Updated: 2022/10/27 11:39:05 by gmeoli           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_print_single_quote(char *string, int fd)
{
	int	index;

	index = 1;
	while (string[index] != '\'')
	{
		write(fd, &string[index], 1);
		index++;
	}
	index++;
	return (index);
}

int	ft_print_double_quote(char *string, int fd)
{
	int	index;

	index = 1;
	while (string[index] != '"')
	{
		if (string[index] == '$')
			index += ft_print_dollar(string + index, fd);
		else
		{
			write(fd, &string[index], 1);
			index++;
		}
	}
	index++;
	return (index);
}

void	ft_print_exit(void)
{
	char	*status;

	status = ft_itoa(*g_exit_status);
	write (1, status, ft_strlen(status));
	free(status);
}

int	ft_check_quote(char *str)
{
	int	index;
	int	flag;

	index = 0;
	flag = 1;
	while (str[index])
	{
		if (str[index] == '\'')
		{
			flag *= -1;
			index++;
			while (str[index])
			{
				if (str[index] == '\'')
				{
					flag *= -1;
					break ;
				}
				index++;
			}
		}
		else if (str[index] == '\"')
		{
			flag *= -1;
			index++;
			while (str[index])
			{
				if (str[index] == '\"')
				{
					flag *= -1;
					break ;
				}
				index++;
			}
		}
		index++;
	}
	return (flag);
}
