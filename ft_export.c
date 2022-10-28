/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masebast <masebast@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/09 10:59:12 by masebast          #+#    #+#             */
/*   Updated: 2022/10/28 16:07:58 by masebast         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_check_equal_presence(char *string)
{
	int	index;

	index = 0;
	while (string[index])
	{
		if (string[index] == '=')
			return (1);
		index++;
	}
	return (0);
}

int	ft_modify_var(char *new_couple, char **envp)
{
	int		index;
	char	**couple;

	index = 0;
	couple = ft_split(new_couple, '=');
	while (envp[index])
	{
		if (ft_strncmp(envp[index], couple[0], ft_strlen(couple[0])) == 0)
		{
			ft_memcpy(envp[index], new_couple, ft_strlen(new_couple));
			envp[index][ft_strlen(new_couple)] = '\0';
			ft_free_matrix(couple);
			return (1);
		}
		index++;
	}
	ft_free_matrix(couple);
	return (0);
}

int	ft_append_new_key(char **envp, char *new_key)
{
	int	index;

	index = 0;
	while (envp[index])
	{
		if (ft_strncmp(envp[index], new_key, ft_strlen(new_key)) == 0)
			return (1);
		index++;
	}
	envp[index] = ft_strdup(new_key);
	envp[index + 1] = NULL;
	return (0);
}

void	ft_export2(t_command *c_s, char **envp, int *m_i, int *exit_flag)
{
	int	modified_flag;

	while (c_s->word_matrix[*m_i])
	{
		modified_flag = 0;
		if (ft_check_char(c_s->word_matrix[*m_i]) == 0)
		{
			if (ft_check_equal_presence(c_s->word_matrix[*m_i]) == 1)
			{
				modified_flag = ft_modify_var(c_s->word_matrix[*m_i], envp);
				if (modified_flag == 0)
					ft_append_new_key(envp, c_s->word_matrix[*m_i]);
			}
			else
				ft_append_new_key(envp, c_s->word_matrix[*m_i]);
		}
		else
		{
			ft_export_error(c_s->word_matrix[*m_i]);
			*exit_flag = 1;
		}
		m_i++;
	}
}

int	ft_export(t_command *c_s, char **envp)
{
	int	m_i;
	// int	modified_flag;
	int	exit_flag;

	m_i = 0;
	exit_flag = 0;
	if (c_s->word_matrix[1] && ft_strncmp(c_s->word_matrix[1], ">>\0", 3) != 0 \
		&& ft_strncmp(c_s->word_matrix[1], ">\0", 2) != 0 \
		&& ft_strncmp(c_s->word_matrix[1], "<<\0", 3) != 0 \
		&& ft_strncmp(c_s->word_matrix[1], "<\0", 2) != 0)
	{
		ft_export2(c_s, envp, &m_i, &exit_flag);
		// while (c_s->word_matrix[++m_i])
		// {
		// 	modified_flag = 0;
		// 	if (ft_check_char(c_s->word_matrix[m_i]) == 0)
		// 	{
		// 		if (ft_check_equal_presence(c_s->word_matrix[m_i]) == 1)
		// 		{
		// 			modified_flag = ft_modify_var(c_s->word_matrix[m_i], envp);
		// 			if (modified_flag == 0)
		// 				ft_append_new_key(envp, c_s->word_matrix[m_i]);
		// 		}
		// 		else
		// 			ft_append_new_key(envp, c_s->word_matrix[m_i]);
		// 	}
		// 	else
		// 	{
		// 		ft_export_error(c_s->word_matrix[m_i]);
		// 		exit_flag = 1;
		// 	}
		// }
		if (exit_flag == 0)
			*g_exit_status = 0;
		else
			*g_exit_status = 1;
		return (0);
	}
	else
	{
		*g_exit_status = ft_buble_sort_env(envp);
		return (0);
	}
	*g_exit_status = 1;
	return (1);
}
