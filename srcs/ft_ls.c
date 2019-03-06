/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: midrissi <midrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 15:30:46 by midrissi          #+#    #+#             */
/*   Updated: 2019/03/06 22:58:03 by aben-azz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

char g_flags = 0;
void		list_dir(DIR *dir)
{
	t_dirent	*d;
	size_t		size;

	size = 10;
	while ((d = readdir(dir)))
	{
		if (size < ft_strlen(d->d_name))
			size = ft_strlen(d->d_name) + 1;
		if (*(d->d_name) != '.' || (g_flags & F_DOT))
			ft_printf("%-*s", size, d->d_name);
	}
	ft_printf("\n");
	closedir(dir);
}

// void	perms(t_file *file, int mode)
// {
// 	file->perms[0] = 0;
// 	(S_ISREG(mode)) && (file->perms[0] = '-');
// 	(S_ISDIR(mode)) && (file->perms[0] = 'd');
// 	(S_ISBLK(mode)) && (file->perms[0] = 'b');
// 	(S_ISCHR(mode)) && (file->perms[0] = 'c');
// 	(S_ISFIFO(mode)) && (file->perms[0] = 'p');
// 	(S_ISLNK(mode)) && (file->perms[0] = 'l');
// 	(S_ISSOCK(mode)) && (file->perms[0] = 's');
// 	file->perms[0] || (file->perms[0] = '?');
// 	file->perms[1] = ((mode & S_IRUSR) ? 'r' : '-');
// 	file->perms[2] = ((mode & S_IWUSR) ? 'w' : '-');
// 	file->perms[3] = ((mode & S_IXUSR) ? 'x' : '-');
// 	file->perms[4] = ((mode & S_IRGRP) ? 'r' : '-');
// 	file->perms[5] = ((mode & S_IWGRP) ? 'w' : '-');
// 	file->perms[6] = ((mode & S_IXGRP) ? 'x' : '-');
// 	file->perms[7] = ((mode & S_IROTH) ? 'r' : '-');
// 	file->perms[8] = ((mode & S_IWOTH) ? 'w' : '-');
// 	file->perms[9] = ((mode & S_IXOTH) ? 'x' : '-');
// }


char get_correct_char(int mode, int is_exec, int is_sticky)
{
	return (mode & is_exec ? "xs"[mode & is_sticky] : "-S"[mode & is_sticky]);
}


char third_permission(int mode, char type_user)
{
	if (type_user == 'u')
		return (get_correct_char(mode, S_IXUSR, S_ISUID));
	else if (type_user == 'g')
		return (get_correct_char(mode, S_IXGRP, S_ISGID));
	else
		return ((char[2][2]){"-T", "xt"}[mode & S_IXOTH][mode & S_ISTXT]);
}

char get_extended(t_file file)
{
	(void)file;
	return ('@');
}
t_file		create_file(char *name)
{
	t_file		file;

	stat(name, &(file.stats));
	file.name = name;
	file.perms[0] = 0;

	/*TYPE OF FILE*/
	(S_ISREG(file.stats.st_mode)) && (file.perms[0] = '-');
	(S_ISDIR(file.stats.st_mode)) && (file.perms[0] = 'd');
	(S_ISBLK(file.stats.st_mode)) && (file.perms[0] = 'b');
	(S_ISCHR(file.stats.st_mode)) && (file.perms[0] = 'c');
	(S_ISFIFO(file.stats.st_mode)) && (file.perms[0] = 'p');
	(S_ISLNK(file.stats.st_mode)) && (file.perms[0] = 'l');
	(S_ISSOCK(file.stats.st_mode)) && (file.perms[0] = 's');
	file.perms[0] || (file.perms[0] = '?');

	/*USER*/
	file.perms[1] = ((file.stats.st_mode & S_IRUSR) ? 'r' : '-');
	file.perms[2] = ((file.stats.st_mode & S_IWUSR) ? 'w' : '-');
	file.perms[3] = (third_permission(file.stats.st_mode, 'u'));
	/*file.perms[3] = ((file.stats.st_mode & S_IXUSR) ? 'x',  '-');*/

	/*GROUP*/
	file.perms[4] = ((file.stats.st_mode & S_IRGRP) ? 'r' : '-');
	file.perms[5] = ((file.stats.st_mode & S_IWGRP) ? 'w' : '-');
	file.perms[6] = (third_permission(file.stats.st_mode, 'g'));
	/*file.perms[6] = ((file.stats.st_mode & S_IXGRP) ? 'x' : '-');*/

	/*OTHERS*/
	file.perms[7] = ((file.stats.st_mode & S_IROTH) ? 'r' : '-');
	file.perms[8] = ((file.stats.st_mode & S_IWOTH) ? 'w' : '-');
	file.perms[9] = (third_permission(file.stats.st_mode, 'o'));
	/*file.perms[9] = ((file.stats.st_mode & S_IXOTH) ? 'x' : '-');*/

	/*BONUS*/
	file.perms[10] = get_extended(file);
	return (file);
}

void		print_full_info(t_file file)
{
	ft_printf("%s %ld %s %s %lld %.12s %s\n",
	file.perms,
	file.stats.st_nlink,
	getpwuid(file.stats.st_uid)->pw_name,
	getgrgid(file.stats.st_gid)->gr_name,
	file.stats.st_size,
	ctime(&file.stats.st_mtimespec.tv_sec) + 4,
	file.name);
}

void		print_flags(void)
{
	(g_flags & F_LIST) && (ft_printf("g_flags: F_LIST\n"));
	(g_flags & F_RECURSIVE) && (ft_printf("g_flags: F_RECURSIVE\n"));
	(g_flags & F_DOT) && (ft_printf("g_flags: F_DOT\n"));
	(g_flags & F_REVERSE) && (ft_printf("g_flags: F_REVERSE\n"));
	(g_flags & F_SORT_TIME) && (ft_printf("g_flags: F_SORT TIME\n"));
	(g_flags & F_LAST_ACCESS_TIME) && (ft_printf("g_flags: F_ACCESS TIME\n"));
	(g_flags & F_SORT_OFF) && (ft_printf("g_flags: F_SORT OFF\n"));
	(g_flags & F_COLOR) && (ft_printf("g_flags: F_COLOR\n"));
}

void		set_lsflags(int argc, char **argv)
{
	int a;

	a = -1;
	while (++a < argc)
		if (*argv[a] == '-' && (*argv[a]++))
			while (*argv[a] != ' ' && *argv[a])
			{
				if (~ft_indexof(LSFLAGS, *argv[a]))
					g_flags |= (1 << (ft_indexof(LSFLAGS, *argv[a]) - 1));
				(void)*argv[a]++;
			}
}

int			main(int argc, char **argv)
{
	DIR			*dir;
	t_file		file;

	if (argc < 2)
		return (0);
	set_lsflags(argc, argv);
	print_flags();
	dir = opendir(argv[2]);
	list_dir(dir);
	file = create_file(argv[2]);
	print_full_info(file);
	return (0);
}
