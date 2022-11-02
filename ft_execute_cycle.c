/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute_cycle.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masebast <masebast@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 19:44:46 by masebast          #+#    #+#             */
/*   Updated: 2022/11/02 19:44:59 by masebast         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_execute_cycle(t_command *c_s, char **envp)
{
	char	*swap;
	char	*sub_readline;

	swap = NULL;
	sub_readline = NULL;
	c_s->command_string = readline("minishell$ ");
	if (!c_s->command_string)
		ft_exit_on_signal();
	if (c_s->command_string[0] != '\0')
	{
		if (ft_check_quote(c_s->command_string) != 1)
		{
			add_history(c_s->command_string);
			printf("error: close your quotes\n");
			free(c_s->command_string);
			return ;
		}
		if (ft_check_syntax(c_s->command_string) == 1)
		{
			add_history(c_s->command_string);
			free(c_s->command_string);
			return ;
		}
		else if (ft_check_syntax(c_s->command_string) == 2)
		{
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
		add_history(c_s->command_string);
		c_s->total_pipes = ft_count_pipes(c_s->command_string);
		c_s->pipe_matrix = ft_split_pipes(c_s->command_string, '|');
		if (c_s->total_pipes > 1)
			ft_manage_pipes(c_s, envp);
		else
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
	}
	else
		free(c_s->command_string);
}
