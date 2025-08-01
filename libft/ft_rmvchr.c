/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 14:45:38 by tsilveir          #+#    #+#             */
/*   Updated: 2025/06/16 14:45:41 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_rmvchr(char *str, char *char_to_remove)
{
	int	i;

	if (!str)
		return ;
	i = -1;
	while (str[++i])
	{
		if(&str[i] == char_to_remove)
			break;
	}
	while (str[i])
	{
		str[i] = str[i + 1];
		i++;
	}
}
