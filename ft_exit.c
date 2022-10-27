/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masebast <masebast@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/06 14:29:36 by gmeoli            #+#    #+#             */
/*   Updated: 2022/10/27 17:33:11 by masebast         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_exit(t_command *c_s)
{
	printf("exit\n");
	if (c_s->word_matrix[1])
		printf("exit: %s: numeric argument required\n", c_s->word_matrix[1]);
	if (c_s->pipe_matrix)
		ft_free_matrix(c_s->pipe_matrix);
	if (c_s->word_matrix)
		ft_free_matrix(c_s->word_matrix);
	if (c_s->command_string)
		free(c_s->command_string);
	free(g_exit_status);
	exit(0);
}
