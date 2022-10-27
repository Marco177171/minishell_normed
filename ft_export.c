/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masebast <masebast@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/09 10:59:12 by masebast          #+#    #+#             */
/*   Updated: 2022/10/25 19:02:58 by masebast         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ft_create_envp2(char **envp)
{
	int		index;
	char	**envp2;

	index = 0;
	while (envp[index])
		index++;
	envp2 = malloc(sizeof(char *) * index + 1);
	envp2[index] = NULL;
	if (!envp2)
		return (NULL);
	index = 0;
	while (envp[index])
	{
		envp2[index] = ft_strdup(envp[index]);
		index++;
	}
	return (envp2);
}

void	ft_print_envp2(char **envp2)
{
	int		i;
	char	**couple;

	i = 0;
	while (envp2[i])
	{
		if (ft_check_equal_presence(envp2[i]) == 0)
			printf("declare -x %s\n", envp2[i]);
		else
		{
			couple = ft_split(envp2[i], '=');
			if (couple[1] == NULL)
				printf("declare -x %s=\"\"\n", couple[0]);
			else
				printf("declare -x %s=\"%s\"\n", couple[0], couple[1]);
			ft_free_matrix(couple);
		}
		i++;
	}
}

int	ft_buble_sort_env(char **envp)
{
	int		length;
	int		i;
	int		j;
	char	*tmp;
	char	**envp2;

	length = 0;
	i = 0;
	tmp = 0;
	envp2 = ft_create_envp2(envp);
	while (envp2[length])
		length++;
	while (envp2[i])
	{
		j = i + 1;
		while (envp2[j])
		{
			if (ft_strcmp(envp2[i], envp2[j]) > 0)
			{
				tmp = envp2[i];
				envp2[i] = envp2[j];
				envp2[j] = tmp;
			}
			j++;
		}
		i++;
	}
	ft_print_envp2(envp2);
	ft_free_matrix(envp2);
	return (0);
}

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

int	ft_check_char(char *str)
{
	if ((str[0] >= 65 && str[0] <= 90) || (str[0] >= 97 && str[0] <= 122))
		return (0);
	return (1);
}

int	ft_export(t_command *command_struct, char **envp)
{
	int	matrix_index;
	int	modified_flag;
	int	exit_flag;

	matrix_index = 1;
	exit_flag = 0;
	if (command_struct->word_matrix[1] && ft_strncmp(command_struct->word_matrix[1], ">>\0", 3) != 0
		&& ft_strncmp(command_struct->word_matrix[1], ">\0", 2) != 0
		&& ft_strncmp(command_struct->word_matrix[1], "<<\0", 3) != 0
		&& ft_strncmp(command_struct->word_matrix[1], "<\0", 2) != 0)
	{
		while (command_struct->word_matrix[matrix_index])
		{
			modified_flag = 0;
			if (ft_check_char(command_struct->word_matrix[matrix_index]) == 0)
			{
				if (ft_check_equal_presence(command_struct->word_matrix[matrix_index]) == 1)
				{
					modified_flag = ft_modify_var(command_struct->word_matrix[matrix_index], envp);
					if (modified_flag == 0)
						ft_append_new_key(envp, command_struct->word_matrix[matrix_index]);
					matrix_index++;
				}
				else
				{
					ft_append_new_key(envp, command_struct->word_matrix[matrix_index]);
					matrix_index++;
				}
			}
			else
			{
				write(2, "minishell: export: '", 20);
				write(2, command_struct->word_matrix[matrix_index], ft_strlen(command_struct->word_matrix[matrix_index]));
				write(2, "': not a valild identifier\n", 27);
				exit_flag = 1;
				matrix_index++;
			}
		}
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
