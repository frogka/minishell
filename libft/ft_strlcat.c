/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 14:44:30 by tsilveir          #+#    #+#             */
/*   Updated: 2025/06/16 14:44:31 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dsize)
{
	size_t	i;
	size_t	d;

	if (dsize == 0 && !dst)
		return (dsize + ft_strlen(src));
	if (dsize <= ft_strlen(dst))
		return (dsize + ft_strlen(src));
	i = 0;
	d = ft_strlen(dst);
	while (src[i] && dsize - d - 1 > 0)
	{
		dst[d] = src[i];
		d++;
		i++;
	}
	dst[d] = '\0';
	return (ft_strlen(dst) + ft_strlen(&src[i]));
}
