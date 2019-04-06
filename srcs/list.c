/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-azz <aben-azz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/06 12:32:19 by aben-azz          #+#    #+#             */
/*   Updated: 2019/04/06 14:14:59 by aben-azz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void		sort_list(t_list *list)
{
	void	*temp;
	t_list	*j;
	int		sorted;

	sorted = 0;
	while (!sorted)
	{
		j = list;
		sorted = 1;
		while (j)
		{
			if (j->next && ft_strcmp(((t_file *)j->content)->name,
									((t_file *)j->next->content)->name) > 0)
			{
				sorted = 0;
				temp = j->content;
				j->content = j->next->content;
				j->next->content = temp;
			}
			j = j->next;
		}
	}
}

void		list_insert(t_list **head, t_list **tail, t_list *needle)
{
	t_list *curr;
	t_list *prev;

	if (!tail || !head || !needle)
		return ;
	if (!(*head))
	{
		ft_lstadd(head, needle);
		*tail = *head;
		return ;
	}
	if (ft_strcmp(((t_file *)(*head)->content)->full_path, ((t_file *)needle->content)->full_path) >= 0)
	{
		ft_lstadd(head, needle);
		return ;
	}
	if (ft_strcmp(((t_file *)(*tail)->content)->full_path, ((t_file *)needle->content)->full_path) <= 0)
	{
		(*tail)->next = needle;
		*tail = needle;
		return ;
	}
	curr = (*head)->next;
	prev = *head;
	while (curr)
	{
		if (ft_strcmp(((t_file *)(curr)->content)->full_path, ((t_file *)needle->content)->full_path) >= 0)
		{
			prev->next = needle;
			needle->next = curr;
			return ;
		}
		curr = curr->next;
		prev = prev->next;
	}
}

void		list_dir(DIR *dir, t_list **head, t_list **tail, char *path, int i)
{
	t_dirent	*d;
	t_list		*list;
	t_file		file;

	list = NULL;
	while ((d = readdir(dir)))
	{
		file = create_file(d->d_name, path);
		file.size = ft_strlen(d->d_name);
		file.id = i;
		list = ft_lstnew((void *)&file, sizeof(t_file));
		list == NULL ? exit(1) : 0;
		list_insert(head, tail, list);
	}
	closedir(dir);
}
