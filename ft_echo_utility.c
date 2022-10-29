/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo_utility.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masebast <masebast@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/20 14:50:36 by masebast          #+#    #+#             */
/*   Updated: 2022/10/29 19:20:29 by masebast         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_doll_arg(char *c, int *index, int fd)
{
	char	env_var_name[1024];
	int		env_var_len;
	char	*env;

	env_var_len = 0;
	if (*c++ == '?')
	{
		c++;
		ft_print_exit();
		return ;
	}
	while (*c != ' ' && *c && *c != '"'
		&& *c != '$' && *c != '\'' && *c)
		env_var_name[env_var_len++] = *c++;
	env_var_name[env_var_len] = '\0';
	env = getenv(env_var_name);
	if (env != NULL)
	{
		write(fd, env, strlen(env));
		return ;
	}
	else if (!env_var_name[0])
		write(fd, "$", 1);
	return ;
}

int	ft_print_doll(char *str, int fd)
{
	// char	env_var_name[1024];
	// int		env_var_len;
	int		index;
	// char	*env;

	// env_var_len = 0;
	index = 0;
	if (str[index + 1] == '"' || str[index + 1] == ' '
		|| str[index + 1] == '$' || str[index + 1] == '\0')
		return (write(fd, &str[index], 1));
	else
	{
		ft_doll_arg(&str[index], &index, fd);
		// if (str[++index] == '?')
		// {
		// 	index++;
		// 	ft_print_exit();
		// 	return (index);
		// }
		// while (str[index] != ' ' && str[index] && str[index] != '"'
		// 	&& str[index] != '$' && str[index] != '\'' && str[index])
		// 	env_var_name[env_var_len++] = str[index++];
		// env_var_name[env_var_len] = '\0';
		// env = getenv(env_var_name);
		// if (env != NULL)
		// 	write(fd, env, strlen(env));
		// else if (!env_var_name[0])
		// 	write(fd, "$", 1);
	}
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

// if (pipe[index] == ' ')
// 	index++;
// else
// {
// 	while (pipe[index] != ' ')
// 	{
// 		if (pipe[index] == '\'' || pipe[index] == '\"')
// 			index++;
// 		else
// 		{
// 			new_pipe[result_index] = pipe[index];
// 			result_index++;
// 			index++;
// 		}
// 	}
// 	break ;
// }
// index++;
