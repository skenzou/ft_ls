/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-azz <aben-azz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/06 12:38:49 by aben-azz          #+#    #+#             */
/*   Updated: 2019/04/12 05:08:52 by midrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int		compare_folder_t(t_list *first, t_list *second)
{
	int ret;

	ret = ((t_file *)first->content)->time -
					((t_file *)second->content)->time;
	if (!ret)
		ret = ((t_file *)first->content)->ntime -
					((t_file *)second->content)->ntime;
	if (!ret)
		ret = ft_strcmp(((t_file *)second->content)->full_path,
					((t_file *)first->content)->full_path);

	return (ret);
}

void 			print_err(char *name)
{
	char *err;

	err = ft_strjoin("ls: ", name);
	perror(err);
	ft_strdel(&err);
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

static void		cat_fullpath(t_file *file, char *name, char *path)
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

static void 		fill_uids(t_file *file)
{
	t_passwd			*pwd;
	t_group				*group;

	if ((pwd = getpwuid(file->stats.st_uid)))
		file->pwd = ft_strdup(pwd->pw_name);
	else
	{
		if (!(file->pwd = ft_utoa_base(file->stats.st_uid, 10, 0)))
			exit(1);
	}
	if ((group = getgrgid(file->stats.st_gid)))
		file->group = ft_strdup(group->gr_name);
	else
	{
		if (!(file->group = ft_utoa_base(file->stats.st_gid, 10, 0)))
			exit(1);
	}
}

t_file			create_file(char *name, char *path)
{
	t_file		file;

	ft_bzero((void *)&file, sizeof(t_file));
	cat_fullpath(&file, name, path);
	lstat(file.full_path, &(file.stats));
	fill_uids(&file);
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
