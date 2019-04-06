/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: midrissi <midrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 15:30:46 by midrissi          #+#    #+#             */
/*   Updated: 2019/04/06 14:15:01 by aben-azz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

char g_flags = 0;
char g_multiarg = 0;

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

void 	print_list(t_list *files)
{
	t_file file;

	printf("====================================================\n");
	while (files)
	{
		file = *((t_file *)files->content);
		printf("files.id %d -> %s\n", file.id, file.full_path);
		files = files->next;
	}
	printf("====================================================\n");
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
	//file.perms[10] = get_extended(file);
	file.perms[10] = ' ';
	file.perms[11] = '\0';
	return (file);
}

static	void		ft_ls(int argc, char **names)
{
	DIR			*dir;
	t_list		*head;
	t_list		*tail;
	t_list		*fiflnks;
	int			i;

	head = NULL;
	tail = NULL;
	fiflnks = NULL;
	i = argc == 1 ? 0 : 1;
	while (i < argc)
	{
		dir = opendir(names[i]);
		if (!dir)
			handle_notdir(names[i], &fiflnks);
		else
			list_dir(dir, &head, &tail, names[i], i);
		i++;
	}
	ft_lstrev(&fiflnks);
	handle_fiflnks(fiflnks, head);
	if (!g_flags || (~g_flags & F_LIST))
		simple_print_col(head);
	else
		print_full_info(head);
}

static	int		sort_args(int argc, char **argv)
{
	int		sorted;
	int		j;
	char	*temp;

	sorted = 0;
	while (!sorted)
	{
		j = 1;
		sorted = 1;
		while (j < argc)
		{
			if (argv[j + 1] && ft_strcmp(argv[j], argv[j + 1]) > 0)
			{
				sorted = 0;
				temp = argv[j];
				argv[j] = argv[j + 1];
				argv[j + 1] = temp;
			}
			j++;
		}
	}
	return (1);
}

int					main(int argc, char **argv)
{
	set_lsflags(argc, argv);
	(g_flags > 0) && (argv++);
	argc = argc - (g_flags > 0);
	(g_multiarg = argc > 2) && sort_args(argc, argv);
	if (argc == 1 && (argv[0] = "."))
		ft_ls(argc, argv);
	else
		ft_ls(argc, argv);
	// print_flags();
	return (0);
}
