/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redirections.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masebast <masebast@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/07 15:45:04 by masebast          #+#    #+#             */
/*   Updated: 2022/10/31 18:55:51 by masebast         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ft_decrease_word_matrix(char **word_matrix)
{
	int		index;
	int		found;
	int		temp_index;
	char	**temp_matrix;

	index = 0;
	found = 0;
	while (word_matrix[index])
		index++;
	temp_matrix = malloc(sizeof(char *) * index);
	index = 0;
	temp_index = 0;
	while (word_matrix[index])
	{
		if ((ft_strncmp(word_matrix[index], ">>\0", 3) == 0
				|| ft_strncmp(word_matrix[index], ">\0", 2) == 0
				|| ft_strncmp(word_matrix[index], "<<\0", 3) == 0
				|| ft_strncmp(word_matrix[index], "<\0", 2) == 0)
			&& found != 1)
		{
			found = 1;
			index += 2;
		}
		else
		{
			temp_matrix[temp_index] = ft_strdup(word_matrix[index]);
			temp_index++;
			index++;
		}
	}
	temp_matrix[temp_index] = NULL;
	ft_free_matrix(word_matrix);
	return (temp_matrix);
}

char	*ft_update_pipe_text(char *pipe)
{
	int		index;
	int		count;
	char	*updated;

	index = 0;
	count = 0;
	while (pipe[index])
	{
		if (pipe[index] == '\'')
		{
			index++;
			count++;
			while (pipe[index] != '\'')
			{
				index++;
				count++;
			}
			index++;
			count++;
		}
		else if (pipe[index] == '"')
		{
			index++;
			count++;
			while (pipe[index] != '"')
			{
				index++;
				count++;
			}
			index++;
			count++;
		}
		else if (pipe[index] == '>' || pipe[index] == '<')
		{
			index++;
			while (pipe[index] != ' ' && pipe[index])
				index++;
			while (pipe[index] == ' ' && pipe[index])
				index++;
			while (pipe[index] != ' ' && pipe[index])
				index++;
		}
		else
		{
			count++;
			index++;
		}
	}
	updated = malloc(sizeof(char) * count + 1);
	updated[count] = '\0';
	index = 0;
	count = 0;
	while (pipe[index])
	{
		if (pipe[index] == '\'')
		{
			updated[count++] = pipe[index++];
			while (pipe[index] != '\'')
				updated[count++] = pipe[index++];
			updated[count++] = pipe[index++];
		}
		else if (pipe[index] == '"')
		{
			updated[count++] = pipe[index++];
			while (pipe[index] != '"')
				updated[count++] = pipe[index++];
			updated[count++] = pipe[index++];
		}
		if (pipe[index] == '>' || pipe[index] == '<')
		{
			index++;
			while (pipe[index] != ' ' && pipe[index])
				index++;
			while (pipe[index] == ' ' && pipe[index])
				index++;
			while (pipe[index] != ' ' && pipe[index])
				index++;
		}
		else
		{
			updated[count] = pipe[index];
			count++;
			index++;
		}
	}
	free(pipe);
	return (updated);
}

void	ft_red_exe(t_command *c_s, int p_i, char **envp, int fd, int stdoutcpy)
{
	c_s->pipe_matrix[p_i] = ft_update_pipe_text(c_s->pipe_matrix[p_i]);
	c_s->word_matrix = ft_decrease_word_matrix(c_s->word_matrix);
	close(STDOUT_FILENO);
	dup2(fd, STDOUT_FILENO);
	ft_recognize_command(c_s, p_i, envp);
	dup2(stdoutcpy, STDOUT_FILENO);
	close(fd);
}

int	ft_find_quotes(char *word)
{
	int		index;
	int		flag;
	char	quote;

	index = -1;
	flag = 0;
	while (word[++index])
	{
		if (word[index] == '\'' || word[index] == '\"')
		{
			flag = 1;
			quote = word[index];
			while (word[++index])
			{
				if (word[index] == quote)
				{
					flag = 0;
					break ;
				}
			}
		}
	}
	return (flag);
}

void	ft_redirect(t_command *c_s, int p_i, char **envp)
{
	int		fd_out;
	int		index;
	int		stdoutcpy;
	int		stdincpy;
	int		fd_in;

	fd_out = 0;
	fd_in = 0;
	index = 0;
	stdincpy = dup(0);
	stdoutcpy = dup(1);
	while (c_s->word_matrix[index])
	{
		if (ft_find_quotes(c_s->word_matrix[index]) == 1)
		{
			index++;
			while (ft_find_quotes(c_s->word_matrix[index]) != 1)
				index++;
		}
		if (ft_strcmp(c_s->word_matrix[index], ">>") == 0
			|| ft_strcmp(c_s->word_matrix[index], ">") == 0)
		{
			ft_redirect_output(c_s, p_i, envp, &index);
			break ;
		}
		else if (ft_strcmp(c_s->word_matrix[index], "<<") == 0
			|| ft_strcmp(c_s->word_matrix[index], "<") == 0)
		{
			ft_redirect_input(c_s, p_i, envp, &index);
			break ;
		}
		if (c_s->word_matrix[index + 1] != NULL)
			index++;
		else
			return ;
	}
	dup2(stdincpy, 0);
}
