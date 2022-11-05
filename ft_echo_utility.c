/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo_utility.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmeoli <gmeoli@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/20 14:50:36 by masebast          #+#    #+#             */
/*   Updated: 2022/11/05 18:57:10 by gmeoli           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_doll_arg(char *str, int *index, int fd)
{
	char	env_var_name[1024];
	int		env_var_len;
	char	*env;

	env_var_len = 0;
	if (str[*index] == '?')
	{
		(*index)++;
		ft_print_exit();
		return ;
	}
	while (str[*index] != ' ' && str[*index] && str[*index] != '"'
		&& str[*index] != '$' && str[*index] != '\'' && str[*index])
		env_var_name[env_var_len++] = str[(*index)++];
	env_var_name[env_var_len] = '\0';
	env = getenv(env_var_name);
	if (env != NULL)
		write(fd, env, strlen(env));
	else if (!env_var_name[0])
		write(fd, "$", 1);
}

int	ft_print_doll(char *str, int fd)
{
	int	index;

	index = 1;
	if (str[index] == '"' || str[index] == ' ' || \
		str[index] == '\'' || str[index] == '$' || str[index] == '\0'){
			// printf("|%s|\n", &str[index]);
			return (write(fd, &str[index], 1));
		}
	else if (ft_isalpha(str[index]) || str[index] == '?')
		ft_doll_arg(str, &index, fd);
	return (index);
}

int	ft_fill_new_pipe(char c)
{
	if (c == ' ')
		return (0);
	else if (c == '\'' || c == '\"')
		return (1);
	else
		return (2);
}

char	*ft_adjust_pipe(char *pipe)
{
	int		index;
	int		found;
	int		result_index;
	char	*new_pipe;

	found = 0;
	result_index = 0;
	index = 0;
	new_pipe = malloc (sizeof(char) * ft_strlen(pipe));
	while (pipe[index++])
	{
		if (found == 1 && ft_fill_new_pipe(pipe[index]) == 0)
			break ;
		if (ft_fill_new_pipe(pipe[index]) == 2)
		{
			found = 1;
			new_pipe[result_index++] = pipe[index];
		}
	}
	while (pipe[index])
		new_pipe[result_index++] = pipe[index++];
	new_pipe[result_index] = '\0';
	free(pipe);
	return (new_pipe);
}
