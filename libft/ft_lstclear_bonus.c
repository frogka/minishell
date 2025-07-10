/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 14:40:53 by tsilveir          #+#    #+#             */
/*   Updated: 2025/06/16 14:40:54 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*prev_lst;

	if (!(lst) || !(*lst) || !del)
		return ;
	while (*lst)
	{
		prev_lst = *lst;
		*lst = prev_lst->next;
		ft_lstdelone(prev_lst, del);
	}
}
