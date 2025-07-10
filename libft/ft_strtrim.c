/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 14:45:19 by tsilveir          #+#    #+#             */
/*   Updated: 2025/06/16 14:45:20 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_is_incset(char ch, char const *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (ch == set[i])
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	start;
	size_t	end;
	char	*dest;

	start = 0;
	while (s1[start] && ft_is_incset(s1[start], set))
		start++;
	end = ft_strlen(s1) - 1;
	while (end >= start && ft_is_incset(s1[end], set))
		end--;
	dest = ft_substr(s1, start, end - start + 1);
	return (dest);
}
