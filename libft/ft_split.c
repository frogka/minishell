/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 14:43:49 by tsilveir          #+#    #+#             */
/*   Updated: 2025/06/16 14:43:50 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_cset(char s, char c)
{
	if (s == c)
		return (1);
	else
		return (0);
}

static int	counter_word(char const *s, char c)
{
	int	counter;
	int	i;

	counter = 0;
	i = 0;
	while (s[i])
	{
		if (is_cset(s[i], c) == 0 && (i == 0 || is_cset(s[i - 1], c)))
		{
			counter++;
		}
		i++;
	}
	return (counter);
}

static void	split_loop(char const *s, char c, t_split *sp)
{
	while (sp->index < sp->n_words)
	{
		while (s[sp->i] && is_cset(s[sp->i], c) == 1)
			sp->i++;
		sp->start = sp->i;
		while (s[sp->i] && is_cset(s[sp->i], c) == 0)
			sp->i++;
		sp->dest[sp->index] = ft_substr(s, sp->start, sp->i - sp->start);
		if (!sp->dest[sp->index])
		{
			while (--sp->index >= 0)
				free(sp->dest[sp->index]);
		}
		sp->index++;
	}
	sp->dest[sp->index] = NULL;
}

char	**ft_split(char const *s, char c)
{
	t_split	sp;

	if (!s)
		return (NULL);
	sp.i = 0;
	sp.index = 0;
	sp.n_words = counter_word(s, c);
	sp.dest = malloc(sizeof(char *) * (sp.n_words + 1));
	if (!sp.dest)
		return (NULL);
	split_loop(s, c, &sp);
	return (sp.dest);
}
