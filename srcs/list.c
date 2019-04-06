/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-azz <aben-azz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/06 12:32:19 by aben-azz          #+#    #+#             */
/*   Updated: 2019/04/06 21:20:55 by midrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	list_insert_loop(t_list **head, t_list *needle, char reverse)
{
	t_list *curr;
	t_list *prev;

	curr = (*head)->next;
	prev = *head;
	while (curr)
	{
		if (!reverse && ft_strcmp(((t_file *)(curr)->content)->full_path,
			((t_file *)needle->content)->full_path) >= 0)
		{
			prev->next = needle;
			needle->next = curr;
			return ;
		}
		if (reverse && ft_strcmp(((t_file *)(curr)->content)->full_path,
			((t_file *)needle->content)->full_path) <= 0)
		{
			prev->next = needle;
			needle->next = curr;
			return ;
		}
		curr = curr->next;
		prev = prev->next;
	}
}

void	list_insertrev(t_list **head, t_list **tail, t_list *needle)
{
	if (!tail || !head || !needle)
		return ;
	if (!(*head))
	{
		ft_lstadd(head, needle);
		*tail = *head;
		return ;
	}
	if (ft_strcmp(((t_file *)(*head)->content)->full_path,
		((t_file *)needle->content)->full_path) <= 0)
	{
		ft_lstadd(head, needle);
		return ;
	}
	if (ft_strcmp(((t_file *)(*tail)->content)->full_path,
		((t_file *)needle->content)->full_path) >= 0)
	{
		(*tail)->next = needle;
		*tail = needle;
		return ;
	}
	list_insert_loop(head, needle, 1);
}

void	list_insert(t_list **head, t_list **tail, t_list *needle)
{
	if (!tail || !head || !needle)
		return ;
	if (!(*head))
	{
		ft_lstadd(head, needle);
		*tail = *head;
		return ;
	}
	if (ft_strcmp(((t_file *)(*head)->content)->full_path,
		((t_file *)needle->content)->full_path) >= 0)
	{
		ft_lstadd(head, needle);
		return ;
	}
	if (ft_strcmp(((t_file *)(*tail)->content)->full_path,
		((t_file *)needle->content)->full_path) <= 0)
	{
		(*tail)->next = needle;
		*tail = needle;
		return ;
	}
	list_insert_loop(head, needle, 0);
}

void	list_dir(DIR *dir, t_list **head, t_list **tail, char *path)
{
	t_dirent	*d;
	t_list		*list;
	t_file		file;
	static int	id;

	list = NULL;
	while ((d = readdir(dir)))
	{
		file = create_file(d->d_name, path);
		file.size = ft_strlen(d->d_name);
		file.id = id;
		list = ft_lstnew((void *)&file, sizeof(t_file));
		list == NULL ? exit(1) : 0;
		(g_flags & F_REVERSE) ? list_insertrev(head, tail, list)
								: list_insert(head, tail, list);
	}
	id++;
	closedir(dir);
}
