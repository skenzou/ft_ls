/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-azz <aben-azz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/06 12:32:19 by aben-azz          #+#    #+#             */
/*   Updated: 2019/04/13 01:30:01 by midrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

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

int			insert_asc_r(t_list **head, t_list *needle, int ret)
{
	if (!head || !needle)
		return (1);
	if (!(*head))
	{
		ft_lstadd(head, needle);
		return (1);
	}
	ret = ft_strcmp(((t_file *)(*head)->content)->full_path,
										((t_file *)needle->content)->full_path);
	if (ret < 0)
	{
		ft_lstadd(head, needle);
		return (1);
	}
	return (insert_asc_loop(head, needle, 1));
}

int			insert_asc(t_list **head, t_list *needle, int ret)
{
	if (!head || !needle)
		return (0);
	if (!(*head))
	{
		ft_lstadd(head, needle);
		return (1);
	}
	ret = ft_strcmp(((t_file *)(*head)->content)->full_path,
										((t_file *)needle->content)->full_path);
	if (ret > 0)
	{
		ft_lstadd(head, needle);
		return (1);
	}
	return (insert_asc_loop(head, needle, 0));
}

void		list_dir(DIR *dir, t_list **head, char *path, long ntime)
{
	t_dirent		*d;
	t_list			*list;
	t_file			file;
	time_t			time;

	while ((d = readdir(dir)))
	{
		file = create_file(d->d_name, path);
		if (*(file.name) == '.' && !*(file.name + 1))
		{
			time = file.stats.st_mtimespec.tv_sec;
			ntime = file.stats.st_mtimespec.tv_nsec;
		}
		file.time = time;
		file.ntime = ntime;
		list = ft_lstnew((void *)&file, sizeof(t_file));
		list == NULL ? exit(1) : 0;
		if (g_flags & F_REVERSE)
			(g_flags & F_LAST_MODIF) ? insert_time_r(head, list, 1)
											: insert_asc_r(head, list, 1);
		else
			(g_flags & F_LAST_MODIF) ? insert_time(head, list, 1)
											: insert_asc(head, list, 1);
	}
	closedir(dir);
}
