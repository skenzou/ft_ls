/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-azz <aben-azz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/06 12:23:31 by aben-azz          #+#    #+#             */
/*   Updated: 2019/04/12 04:59:14 by midrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static void		print_name(t_file *file, int size)
{
	if (DEFAULT_COLOR || (g_flags & F_COLOR))
	{
		if (S_ISDIR(file->stats.st_mode))
			lsprint(file->name, file->namesize, size, ANSI_BOLDCYAN);
		else if (S_ISFIFO(file->stats.st_mode))
			lsprint(file->name, file->namesize, size, ANSI_YELLOW);
		else if (S_ISLNK(file->stats.st_mode))
			lsprint(file->name, file->namesize, size, ANSI_PURPLE);
		else if (S_ISSOCK(file->stats.st_mode))
			lsprint(file->name, file->namesize, size, ANSI_GREEN);
		else if (S_ISCHR(file->stats.st_mode))
			lsprint(file->name, file->namesize, size, "\x1b[43m\x1b[34m");
		else if (S_ISBLK(file->stats.st_mode))
			lsprint(file->name, file->namesize, size, "\x1b[46m\x1b[34m");
		else if ((((file->stats.st_mode) & S_IXUSR) == S_IXUSR))
			lsprint(file->name, file->namesize, size, ANSI_RED);
		else
			lsprint(file->name, file->namesize, size, "");
	}
	else
		lsprint(file->name, file->namesize, size, "");
}

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

static int		continue_reading(t_list *head)
{
	t_file	*file;

	while (head)
	{
		file = ((t_file*)head->content);
		if (*(file->name) != '.' || (g_flags & F_DOT))
			if (S_ISDIR(file->stats.st_mode)
					&& ft_strcmp(file->name, ".") && ft_strcmp(file->name, ".."))
				ft_ls_r(file);
		head = head->next;
	}
	return (1);
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

static void		simple_print_name(t_file *file, size_t size, int i, int col)
{
	if (i > file->nbfiles - col && i <= file->nbfiles)
		print_name(file, 0);
	else
		print_name(file, size);
}

static void 	simple_print_loop(t_list *head, size_t size, int col)
{
	int			mod;
	t_list		*files;
	int			i;
	t_file		*file;
	int			nbfiles;

	mod = 0;
	col = get_col(head, &nbfiles);
	while (mod < col && !(i = 0))
	{
		files = head;
		while (files)
		{
			file = (t_file*)files->content;
			if (*(file->name) != '.' || (g_flags & F_DOT))
				if (i++ % col == mod && (file->nbfiles = nbfiles))
					simple_print_name(file, size, i, col);
			files = files->next;
		}
		(++mod <= col && i > 0) ? ft_putchar('\n') : 0;
	}
	(g_flags & F_RECURSIVE) && continue_reading(head);
}

void			simple_print_col(t_list *head, int header)
{
	size_t		size;

	if (!head)
		return ;
	size = get_max_name_length(head) + 1;
	print_head(((t_file *)head->content)->path, header, NULL);
	simple_print_loop(head, size, 0);
}
