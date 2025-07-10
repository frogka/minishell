/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 14:41:34 by tsilveir          #+#    #+#             */
/*   Updated: 2025/06/16 14:41:35 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*lst_start;
	t_list	*lst_node;

	lst_start = NULL;
	if (!lst || !f || !del)
		return (NULL);
	while (lst)
	{
		lst_node = ft_lstnew(f(lst->content));
		if (!lst_node)
		{
			ft_lstclear(&lst_start, del);
			return (NULL);
		}
		ft_lstadd_back(&lst_start, lst_node);
		lst = lst->next;
	}
	return (lst_start);
}
