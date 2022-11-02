/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute_cycle.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masebast <masebast@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 19:44:46 by masebast          #+#    #+#             */
/*   Updated: 2022/11/02 20:07:10 by masebast         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_open_pipe(t_command *c_s)
{
	char	*swap;
	char	*sub_readline;

	swap = NULL;
	sub_readline = NULL;
	while (ft_check_syntax(c_s->command_string) == 2)
	{
		sub_readline = readline("> ");
		swap = ft_strjoin(c_s->command_string, sub_readline);
		free(c_s->command_string);
		c_s->command_string = ft_strdup(swap);
		free(swap);
		free(sub_readline);
		if (ft_check_syntax(c_s->command_string) == 1)
		{
			free(c_s->command_string);
			return ;
		}
	}
}

void	ft_direction(t_command *c_s, char **envp)
{
	c_s->word_matrix = ft_split(c_s->pipe_matrix[0], ' ');
	ft_remove_quotes(c_s->word_matrix[0]);
	if (ft_check_redirection(c_s->word_matrix) == 1)
	{
		ft_redirect(c_s, 0, envp);
		ft_free_matrix(c_s->word_matrix);
		ft_free_matrix(c_s->pipe_matrix);
		free(c_s->command_string);
		return ;
	}
	ft_recognize_command(c_s, 0, envp);
	ft_free_matrix(c_s->word_matrix);
	ft_free_matrix(c_s->pipe_matrix);
	free(c_s->command_string);
}

void	ft_execute_cycle(t_command *c_s, char **envp)
{
	c_s->command_string = readline("minishell$ ");
	if (!c_s->command_string)
		ft_exit_on_signal();
	if (c_s->command_string[0] != '\0')
	{
		if (ft_check_quote(c_s->command_string) != 1)
		{
			ft_error_in_history(c_s);
			return ;
		}
		if (ft_check_syntax(c_s->command_string) == 1)
		{
			ft_add_and_free(c_s);
			return ;
		}
		else if (ft_check_syntax(c_s->command_string) == 2)
			ft_open_pipe(c_s);
		ft_history_pipe_matrix(c_s);
		if (c_s->total_pipes > 1)
			ft_manage_pipes(c_s, envp);
		else
			ft_direction(c_s, envp);
	}
	else
		free(c_s->command_string);
}
