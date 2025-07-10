/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 14:44:24 by tsilveir          #+#    #+#             */
/*   Updated: 2025/06/16 14:44:25 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		len_s1;
	int		len_s2;
	int		i;
	char	*dest;

	len_s1 = (int)ft_strlen(s1);
	len_s2 = (int)ft_strlen(s2);
	dest = malloc((len_s1 + len_s2 + 1) * sizeof(char));
	i = 0;
	if (!dest)
		return (NULL);
	while (i < len_s1)
	{
		dest[i] = s1[i];
		i++;
	}
	i = 0;
	while (i < len_s2)
	{
		dest[i + len_s1] = s2[i];
		i++;
	}
	dest[i + len_s1] = 0;
	return (dest);
}
