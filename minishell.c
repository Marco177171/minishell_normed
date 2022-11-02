/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masebast <masebast@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/01 17:12:49 by masebast          #+#    #+#             */
/*   Updated: 2022/11/02 16:37:54 by masebast         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_recognize_command(t_command *c_s, int p_i, char **envp)
{
	if (ft_strcmp("echo", c_s->word_matrix[0]) == 0)
		return (ft_echo(c_s, p_i));
	else if (ft_strcmp("cd", c_s->word_matrix[0]) == 0)
		return (ft_cd(c_s));
	else if (ft_strcmp("pwd", c_s->word_matrix[0]) == 0)
		return (ft_pwd());
	else if (ft_strcmp("export", c_s->word_matrix[0]) == 0)
		return (ft_export(c_s, envp));
	else if (ft_strcmp("unset", c_s->word_matrix[0]) == 0)
		return (ft_unset(c_s, envp));
	else if (ft_strcmp("env", c_s->word_matrix[0]) == 0)
		return (ft_env(c_s, envp));
	else if (ft_strcmp("exit", c_s->word_matrix[0]) == 0)
	{
		ft_exit(c_s);
		return (0);
	}
	else
		ft_other_commands(c_s, envp);
	return (0);
}

void	ft_remove_quotes(char *command)
{
	int		index;
	int		res_index;
	char	*result;
	char	quote;

	index = -1;
	res_index = 0;
	if (ft_check_quote(command) == 1)
	{
		result = malloc(sizeof(char) * ft_strlen(command) + 1);
		while (command[++index])
		{
			if (command[index] == '"' || command[index] == '\'')
			{
				quote = command[index];
				while (command[++index] != quote)
					result[res_index++] = command[index];
			}
			else
				result[res_index++] = command[index];
		}
		result[res_index] = '\0';
		free (command);
		command = ft_strdup(result);
		free(result);
		return ;
	}
	else
		return ;
}

void	ft_init_struct(t_command *command_struct)
{
	char	*value;

	value = ft_itoa(ft_atoi(getenv("SHLVL")) + 1);
	command_struct->current_shell_level = ft_strjoin("SHLVL=", value);
	free(value);
	command_struct->write_fd = 1;
	command_struct->pipes[0] = 0;
	command_struct->pipes[1] = 1;
	command_struct->total_pipes = 0;
	g_exit_status = malloc(sizeof(int) * 1);
}

int	ft_check_syntax(char *command)
{
	int	index;
	int	flag;

	index = 0;
	flag = 0;
	while (command[index] == ' ' || command[index] == '|')
	{
		if (command[index] == '|')
		{
			ft_syntax_error(command[index]);
			return (1);
		}
		index++;
	}
	index = 0;
	while (command[index])
	{
		if (command[index] == '\'')
		{
			index++;
			while (command[index] != '\'' && command[index])
				index++;
			if (command[index + 1])
				index++;
		}
		else if (command[index] == '"')
		{
			index++;
			while (command[index] != '"' && command[index])
				index++;
			if (command[index + 1])
				index++;
		}
		if (command[index] == '|')
		{
			flag = 1;
			index++;
			if (command[index] == '\0')
				break ;
			else
			{
				while (command[index])
				{
					if (command[index] == '|')
					{
						ft_syntax_error(command[index]);
						return (1);
					}
					else if (command[index] == ' ')
						index++;
					else if (command[index] != ' ' || command[index] != '|')
					{
						flag = 0;
						break ;
					}
				}
			}
		}
		if (command[index] != '\0')
			index++;
	}
	if (flag == 1 && (command[index - 1] == '|' || command[index - 1] == ' '))
		return (2);
	return (0);
}

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

int	main(int ac, char **av, char **envp)
{
	t_command	command_struct;

	av = NULL;
	if (ac == 1)
	{
		ft_init_struct(&command_struct);
		ft_modify_var(command_struct.current_shell_level, envp);
		ft_ctrl_c(envp);
		*g_exit_status = 0;
		while (TRUE)
		{
			ft_manage_signals();
			ft_execute_cycle(&command_struct, envp);
		}
	}
	else
		ft_error();
	return (FALSE);
}
