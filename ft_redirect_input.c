/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redirect_input.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masebast <masebast@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/30 18:44:40 by masebast          #+#    #+#             */
/*   Updated: 2022/10/31 18:50:00 by masebast         ###   ########.fr       */
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

void	ft_heredoc(t_command *c_s, int p_i, char **envp, int incpy, int *i)
{
	char	*interrupter;
	char	*sub_readline;
	int		pipes[2];

	incpy = 0;
	if (c_s->word_matrix[(*i) + 1] == NULL)
	{
		ft_unexpected_token();
		return ;
	}
	else
	{
		interrupter = strdup(c_s->word_matrix[(*i) + 1]);
		c_s->pipe_matrix[p_i] = ft_remove_heredoc(c_s->pipe_matrix[p_i]);
		ft_free_matrix(c_s->word_matrix);
		c_s->word_matrix = ft_split(c_s->pipe_matrix[p_i], ' ');
		pipe(pipes);
		while (1)
		{
			sub_readline = readline("> ");
			if (ft_strcmp(sub_readline, interrupter) == 0)
			{
				write(pipes[1], "\0", 1);
				free(sub_readline);
				break ;
			}
			else
			{
				write(pipes[1], sub_readline, ft_strlen(sub_readline));
				write(pipes[1], "\n", 1);
				free(sub_readline);
			}
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

void	ft_input_redirect(t_command *c_s, int p_i, char **envp, int incpy, int *i)
{
	int	fd;

	if (c_s->word_matrix[(*i) + 1] == NULL)
	{
		ft_unexpected_token();
		return ;
	}
	fd = open(c_s->word_matrix[(*i) + 1], O_RDWR, 0644);
	if (!fd)
	{
		ft_arg_not_found(c_s->word_matrix[(*i)]);
		*g_exit_status = 1;
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
	int	stdincpy;

	stdincpy = dup(0);
	if (ft_strcmp(c_s->word_matrix[(*index)], "<<") == 0)
		ft_heredoc(c_s, p_i, envp, stdincpy, index);
	else if (ft_strcmp(c_s->word_matrix[(*index)], "<") == 0)
		ft_input_redirect(c_s, p_i, envp, stdincpy, index);
}
