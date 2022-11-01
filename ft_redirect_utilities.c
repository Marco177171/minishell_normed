/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redirect_utilities.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masebast <masebast@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 16:21:17 by masebast          #+#    #+#             */
/*   Updated: 2022/11/01 18:01:54 by masebast         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_check_token(t_command *c_s, int *i)
{
	if (c_s->word_matrix[(*i) + 1] == NULL)
	{
		ft_unexpected_token();
		return (1);
	}
	return (0);
}

void	ft_new_matrix(char **w_m, int *i, char **temp_matrix, int *temp_index)
{
	int	found;

	found = 0;
	while (w_m[(*i)])
	{
		if ((ft_strcmp(w_m[(*i)], ">>") == 0 \
			|| ft_strcmp(w_m[(*i)], ">") == 0 \
			|| ft_strcmp(w_m[(*i)], "<<") == 0 \
			|| ft_strcmp(w_m[(*i)], "<") == 0) && found != 1)
		{
			found = 1;
			(*i) += 2;
		}
		else
		{
			temp_matrix[(*temp_index)++] = ft_strdup(w_m[(*i)]);
			(*i)++;
		}
	}
	temp_matrix[(*temp_index)] = NULL;
}

char	**ft_decrease_word_matrix(char **w_m)
{
	int		i;
	int		temp_index;
	char	**temp_matrix;

	i = 0;
	while (w_m[i])
		i++;
	temp_matrix = malloc(sizeof(char *) * i);
	i = 0;
	temp_index = 0;
	ft_new_matrix(w_m, &i, temp_matrix, &temp_index);
	ft_free_matrix(w_m);
	return (temp_matrix);
}
