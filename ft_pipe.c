/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masebast <masebast@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/07 18:19:47 by masebast          #+#    #+#             */
/*   Updated: 2022/10/25 19:04:37 by masebast         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_check_redirection(char **word_struct)
{
	int	index;

	index = 0;
	while (word_struct[index])
	{
		if (strncmp(word_struct[index], ">>\0", 3) == 0
			|| strncmp(word_struct[index], ">\0", 2) == 0
			|| strncmp(word_struct[index], "<<\0", 3) == 0
			|| strncmp(word_struct[index], "<\0", 2) == 0)
			return (1);
		index++;
	}
	return (0);
}

void	ft_manage_pipes(t_command *command_struct, char **envp)
{
	int	index;
	int	pipes[2];
	int	*pid;
	int	stdin_cpy;
	int	stdout_cpy;

	stdin_cpy = dup(0);
	stdout_cpy = dup(1);
	index = 0;
	pid = malloc(sizeof(int) * command_struct->total_pipes);
	while (index < command_struct->total_pipes)
	{
		pipe(pipes);
		command_struct->word_matrix = ft_split(command_struct->pipe_matrix[index], ' ');
		ft_remove_quotes(command_struct->word_matrix[0]);
		pid[index] = fork();
		if (pid[index] == 0)
		{
			if (ft_check_redirection(command_struct->word_matrix) == 1)
			{
				close(pipes[0]);
				ft_redirect(command_struct, index, envp);
				exit(0);
			}
			else
			{
				close(pipes[0]);
				if (index == command_struct->total_pipes - 1)
					dup2(stdout_cpy, STDOUT_FILENO);
				else
					dup2(pipes[1], STDOUT_FILENO);
				ft_recognize_command(command_struct, index, envp);
				exit(0);
			}
		}
		else
		{
			dup2(pipes[0], STDIN_FILENO);
			close(pipes[0]);
			close(pipes[1]);
			ft_free_matrix(command_struct->word_matrix);
		}
		index++;
	}
	dup2(stdin_cpy, 0);
	close(stdin_cpy);
	close(stdout_cpy);
	index = -1;
	while (++index < command_struct->total_pipes)
		waitpid(pid[index], 0, 0);
	free(pid);
	free(command_struct->command_string);
	ft_free_matrix(command_struct->pipe_matrix);
}
