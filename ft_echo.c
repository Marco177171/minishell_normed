/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masebast <masebast@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/03 18:22:55 by masebast          #+#    #+#             */
/*   Updated: 2022/10/27 17:31:41 by masebast         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_print_doll(char *str, int fd)
{
	char	env_var_name[1024];
	int		env_var_len;
	int		index;
	char	*env;

	env_var_len = 0;
	index = 0;
	if (str[index + 1] == '"' || str[index + 1] == ' '
		|| str[index + 1] == '$' || str[index + 1] == '\0')
		return (write(fd, &str[index], 1));
	else
	{
		index++;
		if (str[index] == '?')
		{
			index++;
			ft_print_exit();
			return (index);
		}
		while (str[index] != ' ' && str[index] && str[index] != '"'
			&& str[index] != '$' && str[index] != '\'' && str[index])
		{
			env_var_name[env_var_len] = str[index];
			index++;
			env_var_len++;
		}
		env_var_name[env_var_len] = '\0';
		env = getenv(env_var_name);
		if (env != NULL)
			write(fd, env, strlen(env));
		else if (!env_var_name[0])
			write(fd, "$", 1);
	}
	return (index);
}

char	*ft_adjust_pipe(char *pipe)
{
	int		index;
	int		result_index;
	char	*new_pipe;

	index = 0;
	result_index = 0;
	while (pipe[index])
		index++;
	new_pipe = malloc (sizeof(char) * index);
	index = 0;
	while (pipe[index])
	{
		if (pipe[index] == ' ')
			index++;
		else
		{
			while (pipe[index] != ' ')
			{
				if (pipe[index] == '\'' || pipe[index] == '\"')
					index++;
				else
				{
					new_pipe[result_index] = pipe[index];
					result_index++;
					index++;
				}
			}
			break ;
		}
	}
	while (pipe[index])
	{
		new_pipe[result_index] = pipe[index];
		result_index++;
		index++;
	}
	new_pipe[result_index] = '\0';
	free(pipe);
	return (new_pipe);
}

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
