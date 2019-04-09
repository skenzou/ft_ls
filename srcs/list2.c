/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: midrissi <midrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/09 02:49:05 by midrissi          #+#    #+#             */
/*   Updated: 2019/04/09 09:48:46 by Mohamed          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int		insert_time_id(t_list *curr, t_list *needle, char reverse)
{
	int		id;

	id = ((t_file *)(needle)->content)->id;
	while (curr)
	{
		if (curr->next && ((t_file *)(curr->next)->content)->id == id)
		{
			return (insert_time_loop(&curr, needle, reverse));
		}
		if (!curr->next)
			break ;
		curr = curr->next;
	}
	curr->next = needle;
	return (1);
}

int			insert_time_loop(t_list **head, t_list *needle, char reverse)
{
	t_list	*curr;
	t_list	*prev;
	int		ret;
	int		id;

	curr = (*head)->next;
	prev = *head;
	id = ((t_file *)(needle)->content)->id;
	while (curr && id == ((t_file *)(curr)->content)->id)
	{
		ret = ((t_file *)needle->content)->stats.st_mtimespec.tv_sec -
				((t_file *)curr->content)->stats.st_mtimespec.tv_sec;
		(ret == 0) && (ret = ((t_file *)needle->content)->stats.st_mtimespec.tv_nsec -
						((t_file *)curr->content)->stats.st_mtimespec.tv_nsec);
		if (ret == 0)
		{
			return (insert_asc_loop(&prev, needle, reverse));
		}
		if ((!reverse && ret > 0) || (reverse && ret < 0))
		{
			prev->next = needle;
			needle->next = curr;
			return (1);
		}
		curr = curr->next;
		prev = prev->next;
	}
	if (curr)
	{
		needle->next = curr;
		prev->next = needle;
	}
	else
		prev->next = needle;
	return (1);
}

static int	add_head(t_list **head, t_list *needle)
{
	ft_lstadd(head, needle);
	return (1);
}

int			insert_time_r(t_list **head, t_list *needle, int ret)
{
	int		id;

	if (!head || !needle)
		return (1);
	if (!(*head))
		return (add_head(head, needle));
	id = ((t_file *)(*head)->content)->id - ((t_file *)needle->content)->id;
	ret = ((t_file *)(*head)->content)->stats.st_mtimespec.tv_sec -
			((t_file *)needle->content)->stats.st_mtimespec.tv_sec;
	(ret == 0) && (ret = ((t_file *)(*head)->content)->stats.st_mtimespec.tv_nsec -
							((t_file *)needle->content)->stats.st_mtimespec.tv_nsec);
	if (ret == 0)
	{
		return (insert_asc_r(head, needle, 1));

	}
	if (ret > 0 && !id)
	{
		ft_lstadd(head, needle);
		return (1);
	}
	return (insert_time_id(*head, needle, 1));
}

int			insert_time(t_list **head, t_list *needle, int ret)
{
	if (!head || !needle)
		return (0);
	if (!(*head))
		return (add_head(head, needle));
	ret = ((t_file *)(*head)->content)->id - ((t_file *)needle->content)->id;
	if (((t_file *)(*head)->content)->stats.st_mtimespec.tv_sec -
		((t_file *)needle->content)->stats.st_mtimespec.tv_sec < 0 && !ret)
	{
		ft_lstadd(head, needle);
		return (1);
	}
	return (insert_time_id(*head, needle, 0));
}
