/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redirect_utilities.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masebast <masebast@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 16:21:17 by masebast          #+#    #+#             */
/*   Updated: 2022/11/01 16:24:29 by masebast         ###   ########.fr       */
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
