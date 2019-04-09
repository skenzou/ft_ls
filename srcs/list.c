/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-azz <aben-azz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/06 12:32:19 by aben-azz          #+#    #+#             */
/*   Updated: 2019/04/09 03:12:46 by midrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static int	insert_asc_id(t_list *curr, t_list *needle, char reverse)
{
	int		id;

	id = ((t_file *)(curr)->content)->id;
	while (curr)
	{
		if (curr->next && ((t_file *)(curr->next)->content)->id != id)
		{
			return (insert_asc_loop(&curr, needle, reverse));
		}
		if (!curr->next)
			break ;
		curr = curr->next;
	}
	curr->next = needle;
	return (1);
}

int			insert_asc_loop(t_list **head, t_list *needle, char reverse)
{
	t_list	*curr;
	t_list	*prev;
	int		ret;

	curr = (*head)->next;
	prev = *head;
	while (curr)
	{
		ret = ft_strcmp(((t_file *)(curr)->content)->full_path,
				((t_file *)needle->content)->full_path);
		if (ret == 0)
			return (insert_asc_id(curr, needle, reverse));
		if ((!reverse && ret > 0) || (reverse && ret < 0))
		{
			prev->next = needle;
			needle->next = curr;
			return (1);
		}
		curr = curr->next;
		prev = prev->next;
	}
	prev->next = needle;
	return (1);
}

static int	insert_asc_r(t_list **head, t_list **tail, t_list *needle, int ret)
{
	if (!tail || !head || !needle)
		return (1);
	if (!(*head))
	{
		ft_lstadd(head, needle);
		*tail = *head;
		return (1);
	}
	ret = ft_strcmp(((t_file *)(*head)->content)->full_path,
										((t_file *)needle->content)->full_path);
	if (ret < 0)
	{
		ft_lstadd(head, needle);
		return (1);
	}
	if (ret == 0)
		return (insert_asc_id(*head, needle, 1));
	if (ft_strcmp(((t_file *)(*tail)->content)->full_path,
		((t_file *)needle->content)->full_path) > 0)
	{
		(*tail)->next = needle;
		*tail = needle;
		return (1);
	}
	return (insert_asc_loop(head, needle, 1));
}

static int	insert_asc(t_list **head, t_list **tail, t_list *needle, int ret)
{
	if (!tail || !head || !needle)
		return (0);
	if (!(*head))
	{
		ft_lstadd(head, needle);
		*tail = *head;
		return (1);
	}
	ret = ft_strcmp(((t_file *)(*head)->content)->full_path,
										((t_file *)needle->content)->full_path);
	if (ret > 0)
	{
		ft_lstadd(head, needle);
		return (1);
	}
	if (ret == 0)
		return (insert_asc_id(*head, needle, 0));
	if (ft_strcmp(((t_file *)(*tail)->content)->full_path,
		((t_file *)needle->content)->full_path) < 0)
	{
		(*tail)->next = needle;
		*tail = needle;
		return (1);
	}
	return (insert_asc_loop(head, needle, 0));
}

void		list_dir(DIR *dir, t_list **head, t_list **tail, char *path)
{
	t_dirent		*d;
	t_list			*list;
	t_file			file;
	static char		id;

	list = NULL;
	while ((d = readdir(dir)))
	{
		file = create_file(d->d_name, path);
		file.size = ft_strlen(d->d_name);
		file.id = id;
		list = ft_lstnew((void *)&file, sizeof(t_file));
		list == NULL ? exit(1) : 0;
		if (g_flags & F_REVERSE)
			(g_flags & F_LAST_ACCESS_TIME) ? insert_time_r(head, tail, list, 1)
											: insert_asc_r(head, tail, list, 1);
		else
			(g_flags & F_LAST_ACCESS_TIME) ? insert_time(head, tail, list, 1)
											: insert_asc(head, tail, list, 1);
	}
	id++;
	closedir(dir);
}
