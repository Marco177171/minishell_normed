/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masebast <masebast@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/07 18:19:47 by masebast          #+#    #+#             */
/*   Updated: 2022/11/01 15:30:12 by masebast         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_check_redirection(char **word_struct)
{
	int	index;

	index = 0;
	while (word_struct[index])
	{
		if (ft_find_quotes(word_struct[index]) == 1)
		{
			index++;
			while (ft_find_quotes(word_struct[index]) != 1)
				index++;
		}
		if (strncmp(word_struct[index], ">>\0", 3) == 0
			|| strncmp(word_struct[index], ">\0", 2) == 0
			|| strncmp(word_struct[index], "<<\0", 3) == 0
			|| strncmp(word_struct[index], "<\0", 2) == 0)
			return (1);
		index++;
	}
	return (0);
}

void	ft_manage_pipes(t_command *c_s, char **envp)
{
	int	index;
	int	pipes[2];
	int	*pid;
	int	stdin_cpy;
	int	stdout_cpy;

	stdin_cpy = dup(0);
	stdout_cpy = dup(1);
	index = 0;
	pid = malloc(sizeof(int) * c_s->total_pipes);
	while (index < c_s->total_pipes)
	{
		pipe(pipes);
		c_s->word_matrix = ft_split(c_s->pipe_matrix[index], ' ');
		ft_remove_quotes(c_s->word_matrix[0]);
		pid[index] = fork();
		if (pid[index] == 0)
		{
			if (ft_check_redirection(c_s->word_matrix) == 1)
			{
				close(pipes[0]);
				ft_redirect(c_s, index, envp);
				exit(0);
			}
			else
			{
				close(pipes[0]);
				if (index == c_s->total_pipes - 1)
					dup2(stdout_cpy, STDOUT_FILENO);
				else
					dup2(pipes[1], STDOUT_FILENO);
				ft_recognize_command(c_s, index, envp);
				exit(0);
			}
		}
		else
		{
			dup2(pipes[0], STDIN_FILENO);
			close(pipes[0]);
			close(pipes[1]);
			ft_free_matrix(c_s->word_matrix);
		}
		index++;
	}
	dup2(stdin_cpy, 0);
	close(stdin_cpy);
	close(stdout_cpy);
	index = -1;
	while (++index < c_s->total_pipes)
		waitpid(pid[index], 0, 0);
	free(pid);
	free(c_s->command_string);
	ft_free_matrix(c_s->pipe_matrix);
}
