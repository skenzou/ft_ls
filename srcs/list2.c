/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: midrissi <midrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/09 02:49:05 by midrissi          #+#    #+#             */
/*   Updated: 2019/04/10 05:58:23 by midrissi         ###   ########.fr       */
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

static void			insert_id_time(t_list **head, t_list *needle, char reverse)
{
	t_list	*curr;
	t_list	*temp;
	int		ret;

	temp = *head;
	curr = *head;
	while (curr)
	{
		if (curr->next && ((t_file *)curr->content)->id != ((t_file *)(curr->next)->content)->id)
			temp = curr;
		if (((t_file *)curr->content)->name[0] == '.' && ((t_file *)curr->content)->name[1] == 0)
		{
			ret = compare_time(curr, needle);
			if (temp == *head)
			{
				if (ret == 0)
					ret = ft_strcmp(((t_file *)needle->content)->full_path,
								((t_file *)(curr)->content)->full_path);
				if ((!reverse && ret <= 0) || (reverse && ret >= 0))
				{
					ft_lstadd(head, needle);
					return ;
				}
			}
			if (ret == 0)
				ret = ft_strcmp(((t_file *)(curr)->content)->full_path,
							((t_file *)needle->content)->full_path);
			if ((!reverse && ret <= 0) || (reverse && ret >= 0))
			{
				needle->next = temp->next;
				temp->next = needle;
				return ;
			}
		}
		if (!curr->next)
			break ;
		curr = curr->next;
	}
	curr->next = needle;
}

int		insert_time_id(t_list **head, t_list *needle, char reverse)
{
	int		id;
	t_list	*curr;

	id = ((t_file *)(needle)->content)->id;
	curr = *head;
	if (((t_file *)(curr)->content)->id == id)
		return (insert_time_loop(&curr, needle, reverse));
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
	insert_id_time(head, needle, reverse);
	return (1);
}

int			insert_time_asc(t_list **head, t_list *needle, char reverse)
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
		ret = compare_time(needle, curr);
		if (ret != 0)
			break ;
		ret = ft_strcmp(((t_file *)(curr)->content)->full_path,
					((t_file *)needle->content)->full_path);
		if ((!reverse && ret >= 0) || (reverse && ret <= 0))
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
	ret = compare_time(*head, needle);
	if (ret == 0)
	{
		if (ft_strcmp(((t_file *)(*head)->content)->full_path,
							((t_file *)needle->content)->full_path) < 0 && !id)
			ft_lstadd(head, needle);
		else
			insert_time_asc(head, needle, 1);
		return (1);
	}
	if (ret > 0 && !id)
	{
		ft_lstadd(head, needle);
		return (1);
	}
	return (insert_time_id(head, needle, 1));
}

int			insert_time(t_list **head, t_list *needle, int ret)
{
	int id;

	if (!head || !needle)
		return (0);
	if (!(*head))
		return (add_head(head, needle));
	id = ((t_file *)(*head)->content)->id - ((t_file *)needle->content)->id;
	ret = compare_time(*head, needle);
	if (ret == 0)
	{
		if (ft_strcmp(((t_file *)(*head)->content)->full_path,
						((t_file *)needle->content)->full_path) > 0 && !id)
			ft_lstadd(head, needle);
		else
			insert_time_asc(head, needle, 0);
		return (1);
	}
	if (ret < 0 && !id)
	{
		ft_lstadd(head, needle);
		return (1);
	}
	return (insert_time_id(head, needle, 0));
}
