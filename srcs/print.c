/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-azz <aben-azz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/06 12:23:31 by aben-azz          #+#    #+#             */
/*   Updated: 2019/04/06 21:29:57 by midrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static void		print_newline(int size)
{
	if (g_flags & F_LIST)
	{
		if (size == -2)
			ft_putchar('\n');
	}
	else
	{
		if (size == -2)
			ft_putstr("\n\n");
		if (size == -1)
			ft_putchar('\n');
	}
}

static void		print_name(t_file *file, int size)
{
	if (S_ISDIR(file->stats.st_mode))
		ft_printf(ANSI_BOLDCYAN "%-*s" ANSI_RESET, size, file->name);
	else if (S_ISFIFO(file->stats.st_mode))
		ft_printf(ANSI_YELLOW "%-*s" ANSI_RESET, size, file->name);
	else if (S_ISLNK(file->stats.st_mode))
		ft_printf(ANSI_PURPLE "%-*s" ANSI_RESET, size, file->name);
	else if (S_ISSOCK(file->stats.st_mode))
		ft_printf(ANSI_GREEN "%-*s" ANSI_RESET, size, file->name);
	else if (S_ISCHR(file->stats.st_mode))
		ft_printf("\x1b[43m\x1b[34m%-s\x1b[0m%*s",
								file->name, size - ft_strlen(file->name), "");
	else if (S_ISBLK(file->stats.st_mode))
		ft_printf("\x1b[46m\x1b[34m%-s\x1b[0m%*s",
								file->name, size - ft_strlen(file->name), "");
	else if ((((file->stats.st_mode) & S_IXUSR) == S_IXUSR))
		ft_printf(ANSI_RED "%-*s" ANSI_RESET, size, file->name);
	else
		ft_printf("%-*s", size, file->name);
	print_newline(size);
}

void			print_full_info(t_list *files)
{
	t_file	file;
	int		length[4];

	set_max_length(files, length);
	while (files)
	{
		file = *((t_file *)files->content);
		if (*(file.name) == '.' && !(*(file.name + 1)))
		{
			g_multiarg && print_path(file.path);
			ft_printf("total %lld\n", get_totalsize(files));
		}
		if (*(file.name) != '.' || (g_flags & F_DOT))
		{
			ft_printf("%s %*hu %-*s %-*s %*lld %.12s ", file.perms, length[0],
			file.stats.st_nlink, length[2] + 1,
			getpwuid(file.stats.st_uid)->pw_name, length[3] + 1,
			getgrgid(file.stats.st_gid)->gr_name, length[1], file.stats.st_size,
			ctime(&file.stats.st_mtimespec.tv_sec) + 4);
			print_name(&file, check_next(files->next, 0));
			(S_ISLNK(file.stats.st_mode)) && print_link(&file);
			ft_putchar('\n');
		}
		files = files->next;
	}
}

static t_list	*simple_print_loop(t_list *head, size_t size, int col, int id)
{
	int			mod;
	t_list		*files;
	int			i;
	t_file		file;

	mod = 0;
	while (mod < col)
	{
		i = 0;
		files = head;
		while (files)
		{
			file = *((t_file*)files->content);
			if (*(file.name) != '.' || (g_flags & F_DOT))
				if (i++ % col == mod)
					print_name(&file, size);
			files = files->next;
			if (files && ((t_file *)files->content)->id != id)
				break ;
		}
		mod++;
		if (mod < col)
			ft_putchar('\n');
	}
	return (files);
}

void			simple_print_col(t_list *head)
{
	size_t		size;
	int			col;
	int			id;

	if (!head)
		return ;
	size = get_max_name_length(head) + 1;
	col = get_col(head);
	id = ((t_file *)head->content)->id;
	if (g_multiarg)
		print_path(((t_file *)head->content)->path);
	if ((head = simple_print_loop(head, size, col, id)))
	{
		ft_putendl("\n");
		simple_print_col(head);
	}
	else
		ft_putchar('\n');
}
