/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-azz <aben-azz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/06 12:38:49 by aben-azz          #+#    #+#             */
/*   Updated: 2019/04/11 08:08:15 by Mohamed          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int				print_link(t_file *file)
{
	char	buff[MAX_PATH_LEN];

	ft_bzero(buff, MAX_PATH_LEN);
	if (readlink(file->full_path, buff, MAX_PATH_LEN) >= 0)
		ft_printf(" -> %s", buff);
	else
		exit(1);
	return (0);
}

int				check_next(t_list *list, int size, int id)
{
	t_file file;

	if (!list)
		return (-1);
	file = *((t_file*)list->content);
	if (id != file.id)
		return (-2);
	return (size);
}

int				print_head(char *path, t_list *files)
{
	int		totalsize;

	totalsize = get_totalsize(files);
	if (g_multiarg && path)
	{
		ft_putstr(path);
		ft_putstr(":\n");
	}
	if (files && totalsize != -1)
		ft_printf("total %lld\n", totalsize);
	return (1);
}

static void		set_perms(t_file *file)
{
	file->perms[0] || (file->perms[0] = '?');
	file->perms[1] = "-r"[(file->stats.st_mode & S_IRUSR) > 0];
	file->perms[2] = "-w"[(file->stats.st_mode & S_IWUSR) > 0];
	file->perms[3] = third_permission(file->stats.st_mode, 'u');
	file->perms[4] = "-r"[(file->stats.st_mode & S_IRGRP) > 0];
	file->perms[5] = "-w"[(file->stats.st_mode & S_IWGRP) > 0];
	file->perms[6] = third_permission(file->stats.st_mode, 'g');
	file->perms[7] = "-r"[(file->stats.st_mode & S_IROTH) > 0];
	file->perms[8] = "-w"[(file->stats.st_mode & S_IWOTH) > 0];
	file->perms[9] = third_permission(file->stats.st_mode, 'o');
	file->perms[10] = get_extended(file);
	file->perms[11] = '\0';
}

t_file			create_file(char *name, char *path)
{
	t_file		file;

	ft_bzero((void *)&file, sizeof(t_file));
	cat_fullpath(&file, name, path);
	lstat(file.full_path, &(file.stats));
	file.perms[0] = 0;
	(S_ISREG(file.stats.st_mode)) && (file.perms[0] = '-');
	(S_ISDIR(file.stats.st_mode)) && (file.perms[0] = 'd');
	(S_ISBLK(file.stats.st_mode)) && (file.perms[0] = 'b');
	(S_ISCHR(file.stats.st_mode)) && (file.perms[0] = 'c');
	(S_ISFIFO(file.stats.st_mode)) && (file.perms[0] = 'p');
	(S_ISLNK(file.stats.st_mode)) && (file.perms[0] = 'l');
	(S_ISSOCK(file.stats.st_mode)) && (file.perms[0] = 's');
	set_perms(&file);
	return (file);
}
