/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masebast <masebast@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/08 19:17:10 by gmeoli            #+#    #+#             */
/*   Updated: 2022/10/25 18:56:30 by masebast         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(t_command *command_struct, char **envp)
{
	int	index;

	command_struct = NULL;
	index = 0;
	if (!envp)
	{
		*g_exit_status = 1;
		return (1);
	}
	else
	{
		while (envp[index])
		{
			if (ft_check_equal_presence(envp[index]) == 1)
				printf("%s\n", envp[index]);
			index++;
		}
		*g_exit_status = 0;
		return (0);
	}
}
