/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: midrissi <midrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 15:30:46 by midrissi          #+#    #+#             */
/*   Updated: 2019/04/01 20:26:07 by midrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

char g_flags = 0;

char		get_extended(t_file file)
{
	(void)file;
	return ('@');
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

static void		cat_fullpath(t_file *file, char *name, char *path)
{
	ft_bzero(file->full_path, MAX_PATH_LEN);
	ft_strcpy(file->full_path, path);
	ft_strcat(file->full_path, "/");
	ft_strcat(file->full_path, name);
}

t_file			create_file(char *name, char *path)
{
	t_file		file;

	cat_fullpath(&file, name, path);
	lstat(file.full_path, &(file.stats));
	file.name = name;
	file.perms[0] = 0;
	(S_ISREG(file.stats.st_mode)) && (file.perms[0] = '-');
	(S_ISDIR(file.stats.st_mode)) && (file.perms[0] = 'd');
	(S_ISBLK(file.stats.st_mode)) && (file.perms[0] = 'b');
	(S_ISCHR(file.stats.st_mode)) && (file.perms[0] = 'c');
	(S_ISFIFO(file.stats.st_mode)) && (file.perms[0] = 'p');
	(S_ISLNK(file.stats.st_mode)) && (file.perms[0] = 'l');
	(S_ISSOCK(file.stats.st_mode)) && (file.perms[0] = 's');
	file.perms[0] || (file.perms[0] = '?');
	file.perms[1] = "-r"[(file.stats.st_mode & S_IRUSR) > 0];
	file.perms[2] = "-w"[(file.stats.st_mode & S_IWUSR) > 0];
	file.perms[3] = third_permission(file.stats.st_mode, 'u');
	file.perms[4] = "-r"[(file.stats.st_mode & S_IRGRP) > 0];
	file.perms[5] = "-w"[(file.stats.st_mode & S_IWGRP) > 0];
	file.perms[6] = third_permission(file.stats.st_mode, 'g');
	file.perms[7] = "-r"[(file.stats.st_mode & S_IROTH) > 0];
	file.perms[8] = "-w"[(file.stats.st_mode & S_IWOTH) > 0];
	file.perms[9] = third_permission(file.stats.st_mode, 'o');
	file.perms[10] = get_extended(file);
	return (file);
}

void		list_dir(DIR *dir, t_list **begin, char *path)
{
	t_dirent	*d;
	t_list		*list;
	t_file		file;

	list = NULL;
	while ((d = readdir(dir)))
	{
		file = create_file(d->d_name, path);
		file.size = ft_strlen(d->d_name);
		file.name = d->d_name;
		list = ft_lstnew((void *)&file, sizeof(t_file));
		list == NULL ? exit(1) : 0;
		ft_lstadd(begin, list);
	}
	closedir(dir);
}

static void		print_full_info(t_list *files)
{
	t_file	file;

	while (files)
	{
		file = *((t_file *)files->content);
		ft_printf("%s %ld %s %s %lld %.12s %s\n",
			file.perms,
			file.stats.st_nlink,
			getpwuid(file.stats.st_uid)->pw_name,
			getgrgid(file.stats.st_gid)->gr_name,
			file.stats.st_size,
			ctime(&file.stats.st_mtimespec.tv_sec) + 4,
			file.name);
		files = files->next;
	}
}

static void		simple_print(t_list *files)
{
	size_t	size;
	t_file	file;

	size = 10;
	while (files)
	{
		file = *((t_file*)files->content);
		if (size < file.size)
			size = file.size + 1;
		if (*(file.name) != '.' || (g_flags & F_DOT))
		{
			if (S_ISDIR(file.stats.st_mode))
				ft_printf(ANSI_CYAN "%-*s" ANSI_RESET, size, file.name);
			else if (S_ISFIFO(file.stats.st_mode))
				ft_printf(ANSI_YELLOW "%-*s" ANSI_RESET, size, file.name);
			else if (S_ISLNK(file.stats.st_mode))
				ft_printf(ANSI_PURPLE "%-*s" ANSI_RESET, size, file.name);
			else if ((((file.stats.st_mode) & S_IXUSR) == S_IXUSR))
				ft_printf(ANSI_RED "%-*s" ANSI_RESET, size, file.name);
			else
				ft_printf("%-*s", size, file.name);
		}
		files = files->next;
	}
	ft_printf("\n");
}

void			print_flags(void)
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

static void			set_lsflags(int argc, char **argv)
{
	int i;

	i = -1;
	g_flags = 0;
	while (++i < argc)
		if (*argv[i] == '-' && argv[i]++)
			while (*(argv[i]) && *(argv[i]) != ' ')
			{
				if (ft_indexof(LSFLAGS, *argv[i]) >= 0)
					g_flags |= (1 << (ft_indexof(LSFLAGS, *argv[i])));
				argv[i]++;
			}
}

static	void		ft_ls(char *name)
{
	DIR			*dir;
	t_list		*files;
	char		*err;

	files = NULL;
	dir = opendir(name);
	if (!dir)
	{
		err = ft_strjoin("ls: ", name);
		perror(err);
		ft_strdel(&err);
		return ;
	}
	list_dir(dir, &files, name);
	if (!g_flags || (~g_flags & F_LIST))
		simple_print(files);
	else
		print_full_info(files);
}

int					main(int argc, char **argv)
{
	int			i;

	set_lsflags(argc, argv);
	(g_flags > 0) && (argv++);
	i = 1;
	while (i < (argc - (g_flags > 0)))
		ft_ls(argv[i++]);
	if (i == 1)
		ft_ls(".");
	// print_flags();
	return (0);
}
