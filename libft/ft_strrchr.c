/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 14:45:13 by tsilveir          #+#    #+#             */
/*   Updated: 2025/06/16 14:45:14 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int	i;

	if (c == 0)
		return ((char *)&(s[ft_strlen(s)]));
	i = ft_strlen(s)-1;
	while (i >= 0)
	{
		if (s[i] == (unsigned char)c)
			return ((char *)&(s[i]));
		i--;
	}
	return (NULL);
}
