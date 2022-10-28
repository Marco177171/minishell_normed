/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmeoli <gmeoli@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/03 18:22:55 by masebast          #+#    #+#             */
/*   Updated: 2022/10/28 15:31:37 by gmeoli           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(t_command *c_s, int p_i)
{
	int	i;
	int	flag;

	i = 0;
	flag = 0;
	c_s->pipe_matrix[p_i] = ft_adjust_pipe(c_s->pipe_matrix[p_i]);
	if (ft_check_quote(c_s->pipe_matrix[p_i]) == 1)
	{
		while (c_s->pipe_matrix[p_i][i] == ' ')
			i++;
		i += 4;
		while (c_s->pipe_matrix[p_i][i] == ' '
			|| c_s->pipe_matrix[p_i][i] == '-')
		{
			if (c_s->pipe_matrix[p_i][i] == '-'
				&& c_s->pipe_matrix[p_i][i + 1] == 'n'
				&& (c_s->pipe_matrix[p_i][i + 2] == ' '
				|| c_s->pipe_matrix[i + 2] == '\0'))
			{
				i += 3;
				flag = 1;
			}
			else if (c_s->pipe_matrix[p_i][i] == '-'
				&& c_s->pipe_matrix[p_i][i] != 'n')
				break ;
			else
				i++;
		}
		while (c_s->pipe_matrix[p_i][i])
		{
			while (c_s->pipe_matrix[p_i][i] == ' '
				&& c_s->pipe_matrix[p_i][i + 1] == ' ')
				i++;
			if (c_s->pipe_matrix[p_i][i] == '\'')
				i += ft_print_single(c_s->pipe_matrix[p_i] + i, c_s->write_fd);
			else if (c_s->pipe_matrix[p_i][i] == '"')
				i += ft_print_double(c_s->pipe_matrix[p_i] + i, c_s->write_fd);
			else if (c_s->pipe_matrix[p_i][i] == '$')
				i += (ft_print_doll(c_s->pipe_matrix[p_i] + i, c_s->write_fd));
			else if (c_s->pipe_matrix[p_i][i] == '>'
				|| c_s->pipe_matrix[p_i][i] == '<')
				break ;
			else
			{
				write(c_s->write_fd, &c_s->pipe_matrix[p_i][i], 1);
				i++;
			}
		}
		if (flag == 0)
			write(c_s->write_fd, "\n", 1);
		*g_exit_status = 0;
		return (0);
	}
	else
	{
		write(2, "error: close your quotes\n", 25);
		return (1);
	}
}
