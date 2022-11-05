/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmeoli <gmeoli@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/05 19:01:41 by gmeoli            #+#    #+#             */
/*   Updated: 2022/11/05 19:24:18 by gmeoli           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ft_create_envp_copy(char **envp)
{
	int		i;
	int		j;
	char	**result;

	i = 0;
	while (envp[i])
		i++;
	result = malloc(sizeof(char *) * i - 1);
	i = 0;
	j = 0;
	while (envp[i] != NULL)
		result[j++] = ft_strdup(envp[i++]);
	result[i - 1] = NULL;
	return (result);
}

void	ft_init_struct(t_command *command_struct, char **envp)
{
	char	*value;

	command_struct->envp_copy = ft_create_envp_copy(envp);
	value = ft_itoa(ft_atoi(ft_getenv_copy(command_struct->envp_copy)));
	// value = ft_itoa(ft_atoi(getenv("SHLVL")) + 1);
	// command_struct->current_shell_level = ft_strjoin("SHLVL=", value);
	// free(value);
	command_struct->write_fd = 1;
	command_struct->pipes[0] = 0;
	command_struct->pipes[1] = 1;
	command_struct->total_pipes = 0;
	g_exit_status = malloc(sizeof(int) * 1);
	ft_print_matrix(command_struct->envp_copy);
}