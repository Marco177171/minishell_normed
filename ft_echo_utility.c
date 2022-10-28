/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo_utility.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmeoli <gmeoli@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/20 14:50:36 by masebast          #+#    #+#             */
/*   Updated: 2022/10/28 15:31:47 by gmeoli           ###   ########.fr       */
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
