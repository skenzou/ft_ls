/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: midrissi <midrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/09 02:49:05 by midrissi          #+#    #+#             */
/*   Updated: 2019/04/13 00:53:25 by midrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static int		compare_time(t_list *first, t_list *second)
{
	int ret;

	ret = ((t_file *)first->content)->stats.st_mtimespec.tv_sec -
					((t_file *)second->content)->stats.st_mtimespec.tv_sec;
	if (!ret)
		ret = ((t_file *)first->content)->stats.st_mtimespec.tv_nsec -
					((t_file *)second->content)->stats.st_mtimespec.tv_nsec;
	return (ret);
}

int				insert_time_asc(t_list **head, t_list *needle, char reverse)
{
	t_list	*curr;
	t_list	*prev;
	int		ret;

	curr = (*head)->next;
	prev = *head;
	while (curr)
	{
		ret = compare_time(needle, curr);
		if (ret != 0)
			break ;
		ret = ft_strcmp(((t_file *)(curr)->content)->full_path,
					((t_file *)needle->content)->full_path);
		if ((!reverse && ret > 0) || (reverse && ret < 0))
		{
			prev->next = needle;
			needle->next = curr;
			return (1);
		}
		curr = curr->next;
		prev = prev->next;
	}
	needle->next = curr;
	prev->next = needle;
	return (1);
}

int				insert_time_loop(t_list **head, t_list *needle, char reverse)
{
	t_list	*curr;
	t_list	*prev;
	int		ret;

	curr = (*head)->next;
	prev = *head;
	while (curr)
	{
		ret = compare_time(needle, curr);
		if (ret == 0)
			return (insert_time_asc(&prev, needle, reverse));
		if ((!reverse && ret > 0) || (reverse && ret < 0))
		{
			prev->next = needle;
			needle->next = curr;
			return (1);
		}
		curr = curr->next;
		prev = prev->next;
	}
	needle->next = curr;
	prev->next = needle;
	return (1);
}

int				insert_time_r(t_list **head, t_list *needle, int ret)
{
	if (!head || !needle)
		return (1);
	if (!(*head))
	{
		ft_lstadd(head, needle);
		return (1);
	}
	ret = compare_time(*head, needle);
	if (ret == 0)
	{
		if (ft_strcmp(((t_file *)(*head)->content)->full_path,
							((t_file *)needle->content)->full_path) < 0)
			ft_lstadd(head, needle);
		else
			insert_time_asc(head, needle, 1);
		return (1);
	}
	if (ret > 0)
	{
		ft_lstadd(head, needle);
		return (1);
	}
	return (insert_time_loop(head, needle, 1));
}

int				insert_time(t_list **head, t_list *needle, int ret)
{
	if (!head || !needle)
		return (0);
	if (!(*head))
	{
		ft_lstadd(head, needle);
		return (1);
	}
	ret = compare_time(*head, needle);
	if (ret == 0)
	{
		if (ft_strcmp(((t_file *)(*head)->content)->full_path,
						((t_file *)needle->content)->full_path) > 0)
			ft_lstadd(head, needle);
		else
			insert_time_asc(head, needle, 0);
		return (1);
	}
	if (ret < 0)
	{
		ft_lstadd(head, needle);
		return (1);
	}
	return (insert_time_loop(head, needle, 0));
}
