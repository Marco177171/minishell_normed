/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masebast <masebast@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/03 18:22:55 by masebast          #+#    #+#             */
/*   Updated: 2022/10/25 18:55:46 by masebast         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_print_dollar(char *str, int fd)
{
	char	env_var_name[1024];
	int		env_var_len;
	int		index;
	char	*env;

	env_var_len = 0;
	index = 0;
	if (str[index + 1] == '"' || str[index + 1] == ' ' || str[index + 1] == '$' || str[index + 1] == '\0')
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
		while (str[index] != ' ' && str[index] && str[index] != '"' && str[index] != '$' && str[index] != '\'' && str[index])
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

int	ft_echo(t_command *command_struct, int pipe_index)
{
	int	index;
	int	flag;

	index = 0;
	flag = 0;
	command_struct->pipe_matrix[pipe_index] = ft_adjust_pipe(command_struct->pipe_matrix[pipe_index]);
	if (ft_check_quote(command_struct->pipe_matrix[pipe_index]) == 1)
	{
		while (command_struct->pipe_matrix[pipe_index][index] == ' ')
			index++;
		index += 4;
		while (command_struct->pipe_matrix[pipe_index][index] == ' ' || command_struct->pipe_matrix[pipe_index][index] == '-')
		{
			if (command_struct->pipe_matrix[pipe_index][index] == '-' && command_struct->pipe_matrix[pipe_index][index + 1] == 'n' && (command_struct->pipe_matrix[pipe_index][index + 2] == ' ' || command_struct->pipe_matrix[index + 2] == '\0'))
			{
				index += 3;
				flag = 1;
			}
			else if (command_struct->pipe_matrix[pipe_index][index] == '-' && command_struct->pipe_matrix[pipe_index][index] != 'n')
				break ;
			else
				index++;
		}
		while (command_struct->pipe_matrix[pipe_index][index])
		{
			while (command_struct->pipe_matrix[pipe_index][index] == ' ' && command_struct->pipe_matrix[pipe_index][index + 1] == ' ')
				index++;
			if (command_struct->pipe_matrix[pipe_index][index] == '\'')
				index += ft_print_single_quote(command_struct->pipe_matrix[pipe_index] + index, command_struct->write_fd);
			else if (command_struct->pipe_matrix[pipe_index][index] == '"')
				index += ft_print_double_quote(command_struct->pipe_matrix[pipe_index] + index, command_struct->write_fd);
			else if (command_struct->pipe_matrix[pipe_index][index] == '$')
				index += (ft_print_dollar(command_struct->pipe_matrix[pipe_index] + index, command_struct->write_fd));
			else if (command_struct->pipe_matrix[pipe_index][index] == '>' || command_struct->pipe_matrix[pipe_index][index] == '<')
				break ;
			else
			{
				write(command_struct->write_fd, &command_struct->pipe_matrix[pipe_index][index], 1);
				index++;
			}
		}
		if (flag == 0)
			write(command_struct->write_fd, "\n", 1);
		*g_exit_status = 0;
		return (0);
	}
	else
	{
		write(2, "error: close your quotes\n", 25);
		return (1);
	}
}
