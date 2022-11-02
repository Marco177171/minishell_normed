/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_pipes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masebast <masebast@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/20 10:16:17 by masebast          #+#    #+#             */
/*   Updated: 2022/11/02 16:54:07 by masebast         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	skip_quotes(const char *s, char quote)
{
	int	index;

	index = 1;
	if (quote != '\'' && quote != '"')
		return (0);
	while (s[index] != quote && s[index])
		index++;
	return (index + 1);
}

int	count_strings_pipes(const char *s, char c)
{
	int		act_pos;
	int		str_count;

	act_pos = 0;
	str_count = 0;
	while (s[act_pos] != '\0')
	{
		if (s[act_pos] == '\'' || s[act_pos] == '"')
			act_pos += skip_quotes(&s[act_pos], s[act_pos]);
		if (s[act_pos] == c)
			str_count++;
		act_pos++;
	}
	str_count++;
	return (str_count);
}

void	ft_quote_control(const char *s, int *i, char *word)
{
	char	quote;

	if (s[(*i)] == '\'' || s[(*i)] == '"')
	{
		quote = s[(*i)];
		word[(*i)] = s[(*i)];
		(*i)++;
		while (s[(*i)] != quote)
		{
			word[(*i)] = s[(*i)];
			(*i)++;
		}
		word[(*i)] = s[(*i)];
	}
	else
		word[(*i)] = s[(*i)];
}

char	*malloc_strings_pipes(const char *s, char c)
{
	char	*word;
	int		i;

	i = 0;
	while (s[i] && s[i] != c)
	{
		if (s[i] == '\'' || s[i] == '"')
			i += skip_quotes(&s[i], s[i]);
		else
			i++;
	}
	word = (char *)malloc(sizeof(char) * (i + 1));
	if (!word)
		return (NULL);
	i = 0;
	while (s[i] && s[i] != c)
	{
		ft_quote_control(s, &i, word);
		i++;
	}
	word[i] = '\0';
	return (word);
}

char	**ft_split_pipes(const char *s, char c)
{
	int		words;
	char	**tab;
	int		i;

	if (!s)
		return (NULL);
	words = count_strings_pipes(s, c);
	if (words == 1)
		c = '\0';
	tab = (char **)malloc(sizeof(char *) * (words + 1));
	if (!tab)
		return (NULL);
	i = 0;
	while (*s)
	{
		while (*s && *s == c)
		{
			s += skip_quotes(s, *s);
			s++;
		}
		if (*s && *s != c)
		{
			tab[i] = malloc_strings_pipes(s, c);
			i++;
			while (*s && *s != c)
			{
				if (*s == '\'' || *s == '"')
					s += skip_quotes(s, *s);
				else
					s++;
			}
		}
	}
	tab[i] = NULL;
	return (tab);
}
