/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redirect_input.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masebast <masebast@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/30 18:44:40 by masebast          #+#    #+#             */
/*   Updated: 2022/11/01 17:14:30 by masebast         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_remove_heredoc(char *pipe)
{
	char	*updated;
	int		index;

	index = 0;
	while (pipe[index] != '<')
		index++;
	updated = malloc(sizeof(char) * index + 1);
	index = -1;
	while (pipe[++index] != '<')
		updated[index] = pipe[index];
	updated[index] = '\0';
	free(pipe);
	return (updated);
}

int	ft_cycle_here(char *interrupter, int pipes[2])
{
	char	*sub_readline;

	sub_readline = readline("> ");
	if (ft_strcmp(sub_readline, interrupter) == 0)
	{
		write(pipes[1], "\0", 1);
		free(sub_readline);
		return (1);
	}
	else
	{
		write(pipes[1], sub_readline, ft_strlen(sub_readline));
		write(pipes[1], "\n", 1);
		free(sub_readline);
	}
	return (0);
}

void	ft_heredoc(t_command *c_s, int p_i, char **envp, int *i)
{
	char	*interrupter;
	int		pipes[2];

	if (ft_check_token(c_s, i) == 1)
		return ;
	else
	{
		interrupter = strdup(c_s->word_matrix[(*i) + 1]);
		c_s->pipe_matrix[p_i] = ft_remove_heredoc(c_s->pipe_matrix[p_i]);
		ft_free_matrix(c_s->word_matrix);
		c_s->word_matrix = ft_split(c_s->pipe_matrix[p_i], ' ');
		pipe(pipes);
		while (1)
		{
			if (ft_cycle_here(interrupter, pipes) == 1)
				break ;
		}
		close(pipes[1]);
		dup2(pipes[0], STDIN_FILENO);
		if (i != 0)
			ft_recognize_command(c_s, p_i, envp);
		close(pipes[0]);
		free(interrupter);
		return ;
	}
}

void	ft_input_redirect(t_command *c_s, int p_i, char **envp, int *i)
{
	int	fd;
	int	incpy;

	incpy = dup(0);
	if (c_s->word_matrix[(*i) + 1] == NULL)
	{
		ft_unexpected_token();
		return ;
	}
	fd = open(c_s->word_matrix[(*i) + 1], O_RDWR, 0644);
	if (fd == -1)
	{
		ft_arg_not_found(c_s->word_matrix[(*i) + 1]);
		*g_exit_status = 1;
		return ;
	}
	c_s->word_matrix = ft_decrease_word_matrix(c_s->word_matrix);
	close(STDIN_FILENO);
	dup2(fd, STDIN_FILENO);
	ft_recognize_command(c_s, p_i, envp);
	dup2(incpy, STDIN_FILENO);
	close(fd);
}

void	ft_redirect_input(t_command *c_s, int p_i, char **envp, int *index)
{
	if (ft_strcmp(c_s->word_matrix[(*index)], "<<") == 0)
		ft_heredoc(c_s, p_i, envp, index);
	else if (ft_strcmp(c_s->word_matrix[(*index)], "<") == 0)
		ft_input_redirect(c_s, p_i, envp, index);
}
