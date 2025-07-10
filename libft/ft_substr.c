/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 14:45:26 by tsilveir          #+#    #+#             */
/*   Updated: 2025/06/16 14:45:27 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_min(size_t len, size_t len_s)
{
	if (len_s <= len)
		return (len_s);
	else
		return (len);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	len_dest;
	char	*dest;

	if (start >= ft_strlen(s))
	{
		dest = malloc((1) * sizeof(char));
		if (!dest)
			return (NULL);
		dest[0] = '\0';
		return (dest);
	}
	i = 0;
	len_dest = ft_min(len, ft_strlen(s) - start);
	dest = malloc((len_dest + 1) * sizeof(char));
	if (!dest)
		return (NULL);
	while (i < (len_dest) && s[i + start])
	{
		dest[i] = s[i + start];
		i++;
	}
	dest[i] = 0;
	return (dest);
}
