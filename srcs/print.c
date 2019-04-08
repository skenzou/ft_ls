/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-azz <aben-azz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/06 12:23:31 by aben-azz          #+#    #+#             */
/*   Updated: 2019/04/09 01:46:28 by midrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

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

static void		print_full_info_name(t_file file, int length[6],
														t_list *files)
{
	if (S_ISCHR(file.stats.st_mode) || S_ISBLK(file.stats.st_mode))
	{
		ft_printf("%s %*hu %-*s  %-*s %*d, %*d %.12s ", file.perms,
		length[0], file.stats.st_nlink, length[2],
		getpwuid(file.stats.st_uid)->pw_name, length[3],
		getgrgid(file.stats.st_gid)->gr_name,
		length[4], major(file.stats.st_rdev),
		length[5], minor(file.stats.st_rdev),
		ctime(&file.stats.st_mtimespec.tv_sec) + 4);
		print_name(&file, check_next(files->next, 0, file.id));
		(S_ISLNK(file.stats.st_mode)) && print_link(&file);
	}
	else
	{
		ft_printf("%s %*hu %-*s  %-*s  %*lld %.12s ", file.perms,
		length[0], file.stats.st_nlink, length[2],
		getpwuid(file.stats.st_uid)->pw_name, length[3],
		getgrgid(file.stats.st_gid)->gr_name, length[1],
		file.stats.st_size,
		ctime(&file.stats.st_mtimespec.tv_sec) + 4);
		print_name(&file, check_next(files->next, 0, file.id));
		(S_ISLNK(file.stats.st_mode)) && print_link(&file);
	}
	ft_putchar('\n');
}

void			print_full_info(t_list *files)
{
	t_file	file;
	int		length[6];
	int		id;

	ft_bzero((void **)&length, sizeof(length));
	set_max_length(files, length);
	files && (id = ((t_file *)files->content)->id);
	(files && id != -1) && print_head(((t_file *)files->content)->path, files);
	while (files)
	{
		file = *((t_file *)files->content);
		if (file.id != id && ((id = file.id) || 1))
			print_head(((t_file *)files->content)->path, files);
		if (*(file.name) != '.' || (g_flags & F_DOT))
			print_full_info_name(file, length, files);
		files = files->next;
	}
}

static t_list	*simple_print_loop(t_list *head, size_t size,
													int col, int id)
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
	print_head(((t_file *)head->content)->path, NULL);
	if ((head = simple_print_loop(head, size, col, id)))
	{
		ft_putendl("\n");
		simple_print_col(head);
	}
	else
		ft_putchar('\n');
}
