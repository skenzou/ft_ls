/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-azz <aben-azz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/06 12:23:31 by aben-azz          #+#    #+#             */
/*   Updated: 2019/04/12 00:03:18 by aben-azz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static void print_name_color(t_file *file, int size)
{
	if (S_ISDIR(file->stats.st_mode))
		ft_printf(ANSI_BOLDCYAN "%-*s"ANSI_RESET, size, file->name);
	else if (S_ISFIFO(file->stats.st_mode))
		ft_printf(ANSI_YELLOW "%-*s" ANSI_RESET, size, file->name);
	else if (S_ISLNK(file->stats.st_mode))
		ft_printf(ANSI_PURPLE "%-*s" ANSI_RESET, size, file->name);
	else if (S_ISSOCK(file->stats.st_mode))
		ft_printf(ANSI_GREEN "%-*s" ANSI_RESET, size, file->name);
	else if (S_ISCHR(file->stats.st_mode))
		ft_printf("\x1b[43m\x1b[34m%-s\x1b[0m%*s",
								file->name, size - file->size, "");
	else if (S_ISBLK(file->stats.st_mode))
		ft_printf("\x1b[46m\x1b[34m%-s\x1b[0m%*s",
								file->name, size - file->size, "");
	else if ((((file->stats.st_mode) & S_IXUSR) == S_IXUSR))
		ft_printf(ANSI_RED "%-*s" ANSI_RESET, size, file->name);
	else
		ft_printf("%-*s", size, file->name);
	print_newline(size);
}

static void		print_name(t_file *file, int size)
{
	if (DEFAULT_COLOR || (g_flags & F_COLOR))
	{
		print_name_color(file, size);
		return ;
	}
	if (S_ISDIR(file->stats.st_mode))
		ft_printf("%-*s", size, file->name);
	else if (S_ISFIFO(file->stats.st_mode))
		ft_printf("%-*s", size, file->name);
	else if (S_ISLNK(file->stats.st_mode))
		ft_printf("%-*s", size, file->name);
	else if (S_ISSOCK(file->stats.st_mode))
		ft_printf("%-*s", size, file->name);
	else if (S_ISCHR(file->stats.st_mode))
		ft_printf("\x1b[43m\x1b[34m%-s\x1b[0m%*s",
								file->name, size - file->size, "");
	else if (S_ISBLK(file->stats.st_mode))
		ft_printf("\x1b[46m\x1b[34m%-s\x1b[0m%*s",
								file->name, size - file->size, "");
	else if ((((file->stats.st_mode) & S_IXUSR) == S_IXUSR))
		ft_printf("%-*s", size, file->name);
	else
		ft_printf("%-*s", size, file->name);
		print_newline(size);
}

static void		print_full_info_name(t_file *file, int length[6])
{
	if (S_ISCHR(file->stats.st_mode) || S_ISBLK(file->stats.st_mode))
	{
		ft_printf("%s %*hu %-*s  %-*s %*d, %*d", file->perms,
		length[0], file->stats.st_nlink,
		length[2], getpwuid(file->stats.st_uid)->pw_name,
		length[3], getgrgid(file->stats.st_gid)->gr_name,
		length[4], major(file->stats.st_rdev),
		length[5], minor(file->stats.st_rdev));
		print_time(file);
		print_name(file, file->size);
		(S_ISLNK(file->stats.st_mode)) && print_link(file);
	}
	else
	{
		ft_printf("%s %*hu %-*s  %-*s  %*lld", file->perms,
		length[0], file->stats.st_nlink,
		length[2], getpwuid(file->stats.st_uid)->pw_name,
		length[3], getgrgid(file->stats.st_gid)->gr_name,
		length[1], file->stats.st_size);
		print_time(file);
		print_name(file, 0);
		(S_ISLNK(file->stats.st_mode)) && print_link(file);
	}
}

static int		continue_reading(t_list *head)
{
	t_file	file;

	while (head)
	{
		file = *((t_file*)head->content);
		if (*(file.name) != '.' || (g_flags & F_DOT))
			if (S_ISDIR(file.stats.st_mode)
					&& ft_strcmp(file.name, ".") && ft_strcmp(file.name, ".."))
				ft_ls_r(&file);
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

static t_list	*simple_print_loop(t_list *head, size_t size, int col)
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
	return (files);
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
