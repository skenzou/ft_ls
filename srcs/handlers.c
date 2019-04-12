/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-azz <aben-azz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/06 12:34:48 by aben-azz          #+#    #+#             */
/*   Updated: 2019/04/12 05:50:42 by midrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void		handle_notdir(char *name, t_list **fiflnks)
{
	t_file	file;
	t_list	*list;

	ft_bzero((void *)&file, sizeof(t_file));
	file = create_file(name, NULL);
	file.namesize = ft_strlen(name);
	if (S_ISFIFO(file.stats.st_mode) || S_ISLNK(file.stats.st_mode) ||
		S_ISREG(file.stats.st_mode) || S_ISBLK(file.stats.st_mode) ||
		S_ISCHR(file.stats.st_mode) || S_ISSOCK(file.stats.st_mode))
	{
		list = ft_lstnew((void *)&file, sizeof(t_file));
		list == NULL ? exit(1) : 0;
		if (g_flags & F_LAST_ACCESS)
			(g_flags & F_REVERSE) ? insert_time_r(fiflnks, list, 1)
														: insert_time(fiflnks, list, 1);
		else
			ft_lstadd(fiflnks, list);
	}
	else
		print_err(name);
}

void		handle_fiflnks(t_list *fiflnks, t_list *head)
{
	if (fiflnks)
	{
		if (!(g_flags & F_DOT))
		{
			g_flags |= F_DOT;
			if (!g_flags || (~g_flags & F_LIST))
				simple_print_col(fiflnks, -1);
			else
				print_full_info(fiflnks, -1);
			g_flags &= ~F_DOT;
		}
		else
		{
			if (!g_flags || (~g_flags & F_LIST))
				simple_print_col(fiflnks, -1);
			else
				print_full_info(fiflnks, -1);
		}
		if (head)
			ft_putchar('\n');
	}
}

void		set_max_length(t_list *l, int len[6])
{
	t_file	*f;
	char	boolean;

	len[4] = 3;
	len[5] = 3;
	boolean = 0;
	while (l)
	{
		f = (t_file *)l->content;
		if (*(f->name) == '.' && !(g_flags & F_DOT) && ((l = l->next) || 1))
			continue ;
		len[0] = ft_max(len[0], ft_intlen_base(f->stats.st_nlink, 10));
		len[1] = ft_max(len[1], ft_intlen_base(f->stats.st_size, 10));
		len[2] = ft_max(len[2], ft_strlen(f->pwd));
		len[3] = ft_max(len[3], ft_strlen(f->group));
		if (S_ISCHR(f->stats.st_mode) || S_ISBLK(f->stats.st_mode))
		{
			boolean = 1;
			len[4] = ft_max(ft_intlen_base(major(f->stats.st_rdev), 10), len[4]);
			len[5] = ft_max(ft_intlen_base(minor(f->stats.st_rdev), 10), len[5]);
		}
		l = l->next;
	}
	boolean && (len[1] = ft_max(len[1], (len[4] + len[5] + 2)));
	len[4] = ft_max(len[1] - len[5] - 1, len[4]);
}

char		third_permission(int m, char type_user)
{
	if (type_user == 'u')
		return (char[2][2]){"-S", "xs"}[(m & S_IXUSR) > 0][(m & S_ISUID) > 0];
	else if (type_user == 'g')
		return (char[2][2]){"-S", "xs"}[(m & S_IXGRP) > 0][(m & S_ISGID) > 0];
	else
		return ((char[2][2]){"-T", "xt"}[(m & S_IXOTH) > 0][(m & S_ISTXT) > 0]);
}
