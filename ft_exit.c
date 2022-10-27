/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masebast <masebast@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/06 14:29:36 by gmeoli            #+#    #+#             */
/*   Updated: 2022/10/26 17:03:52 by masebast         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_exit(t_command *c_struct)
{
	printf("exit\n");
	if (c_struct->word_matrix[1])
		printf("exit: %s: numeric argument required\n", c_struct->word_matrix[1]);
	if (c_struct->pipe_matrix)
		ft_free_matrix(c_struct->pipe_matrix);
	if (c_struct->word_matrix)
		ft_free_matrix(c_struct->word_matrix);
	if (c_struct->command_string)
		free(c_struct->command_string);
	free(g_exit_status);
	exit(0);
}
