/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-azz <aben-azz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/06 12:34:48 by aben-azz          #+#    #+#             */
/*   Updated: 2019/04/06 21:37:46 by midrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void		handle_notdir(char *name, t_list **fiflnks)
{
	char	*err;
	t_file	file;
	t_list	*list;

	ft_bzero((void *)&file, sizeof(t_file));
	file = create_file(name, NULL);
	if (S_ISFIFO(file.stats.st_mode) || S_ISLNK(file.stats.st_mode) ||
		S_ISREG(file.stats.st_mode) || S_ISBLK(file.stats.st_mode) ||
		S_ISCHR(file.stats.st_mode) || S_ISSOCK(file.stats.st_mode))
	{
		list = ft_lstnew((void *)&file, sizeof(t_file));
		list == NULL ? exit(1) : 0;
		ft_lstadd(fiflnks, list);
	}
	else
	{
		err = ft_strjoin("ls: ", name);
		perror(err);
		ft_strdel(&err);
	}
}

void		handle_fiflnks(t_list *fiflnks, t_list *head)
{
	if (fiflnks)
	{
		if (!(g_flags & F_DOT))
		{
			g_flags |= F_DOT;
			if (!g_flags || (~g_flags & F_LIST))
				simple_print_col(fiflnks);
			else
				print_full_info(fiflnks);
			g_flags &= ~F_DOT;
		}
		else
		{
			if (!g_flags || (~g_flags & F_LIST))
				simple_print_col(fiflnks);
			else
				print_full_info(fiflnks);
		}
		if (head)
			ft_putchar('\n');
	}
}

void		set_max_length(t_list *files, int len[4])
{
	t_file	f;

	len[0] = 0;
	len[1] = 0;
	len[2] = 0;
	len[3] = 0;
	while (files)
	{
		f = *((t_file *)files->content);
		len[0] = ft_max(len[0], ft_intlen_base(f.stats.st_nlink, 10));
		len[1] = ft_max(len[1], ft_intlen_base(f.stats.st_size, 10));
		len[2] = ft_max(len[2], ft_strlen(getpwuid(f.stats.st_uid)->pw_name));
		len[3] = ft_max(len[3], ft_strlen(getgrgid(f.stats.st_gid)->gr_name));
		files = files->next;
	}
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

void		cat_fullpath(t_file *file, char *name, char *path)
{
	ft_bzero((void *)&(file->full_path), MAX_PATH_LEN);
	if (path)
	{
		ft_strcpy(file->full_path, path);
		ft_strcat(file->full_path, "/");
	}
	ft_strcat(file->full_path, name);
	file->name = ft_strdup(name);
	!file->name ? exit(1) : 0;
	file->path = path;
}
