/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo_utility_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masebast <masebast@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 15:26:53 by gmeoli            #+#    #+#             */
/*   Updated: 2022/10/28 16:28:05 by masebast         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_print_single(char *string, int fd)
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

int	ft_print_double(char *string, int fd)
{
	int	index;

	index = 1;
	while (string[index] != '"')
	{
		if (string[index] == '$')
			index += ft_print_doll(string + index, fd);
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
	int		index;
	int		flag;
	char	quote;

	index = 0;
	flag = 1;
	// printf("checking %s\n", str);
	while (str[index])
	{
		// printf("char: %c\n", str[index]);
		if (str[index] == '\'' || str[index] == '\"')
		{
			// printf("in\n");
			flag *= -1;
			quote = str[index];
			while (str[index])
			{
				if (str[index] == quote)
				{
					flag *= -1;
					break ;
				}
				index++;
			}
		}
		index++;
		// printf("index = %d\n", index);
	}
	return (flag);
}
