/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 14:40:19 by tsilveir          #+#    #+#             */
/*   Updated: 2025/06/16 14:40:20 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_abs(int n)
{
	if (n < 0)
		n = -n;
	return (n);
}

char	*ft_itoa(int n)
{
	char	temp[12];
	char	final[12];
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	if (n == 0)
		return (ft_strdup("0"));
	if (n < 0)
		final[j++] = '-';
	n = ft_abs(n);
	while (n > 0)
	{
		temp[i++] = n % 10 + '0';
		n = n / 10;
	}
	while (--i >= 0)
		final[j++] = temp[i];
	final[j] = 0;
	return (ft_strdup(final));
}
