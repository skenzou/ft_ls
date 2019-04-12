/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_full_info.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: midrissi <midrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/13 00:29:10 by midrissi          #+#    #+#             */
/*   Updated: 2019/04/13 00:50:53 by midrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static void		print_full_info_name(t_file *file, int length[6])
{
	ft_putstr(file->perms);
	lsputnbr(file->stats.st_nlink, length[0], 1);
	lsprint(file->pwd, ft_strlen(file->pwd), length[2], "");
	write(1, "  ", 2);
	lsprint(file->group, ft_strlen(file->group), length[3], "");
	if (S_ISCHR(file->stats.st_mode) || S_ISBLK(file->stats.st_mode))
	{
		lsputnbr(major(file->stats.st_rdev), length[4], 0);
		ft_putchar(',');
		lsputnbr(minor(file->stats.st_rdev), length[5], 0);
		print_time(file);
		print_name(file, file->namesize);
		(S_ISLNK(file->stats.st_mode)) && print_link(file);
	}
	else
	{
		write(1, " ", 1);
		lsputnbr(file->stats.st_size, length[1], 0);
		print_time(file);
		print_name(file, 0);
		(S_ISLNK(file->stats.st_mode)) && print_link(file);
	}
}

void			print_full_info(t_list *head, int header)
{
	t_file	*file;
	int		length[6];
	t_list	*files;

	if (!head)
		return ;
	files = head;
	ft_bzero((void **)&length, sizeof(length));
	set_max_length(files, length);
	print_head(((t_file *)files->content)->path, header, files);
	while (files)
	{
		file = (t_file *)files->content;
		if (*(file->name) != '.' || (g_flags & F_DOT))
		{
			print_full_info_name(file, length);
			ft_putchar('\n');
		}
		files = files->next;
	}
	if ((g_flags & F_RECURSIVE))
		continue_reading(head);
}
