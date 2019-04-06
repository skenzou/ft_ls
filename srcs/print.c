/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-azz <aben-azz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/06 12:23:31 by aben-azz          #+#    #+#             */
/*   Updated: 2019/04/06 14:15:00 by aben-azz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void			print_name(t_file *file, int size)
{
	if (S_ISDIR(file->stats.st_mode))
		ft_printf(ANSI_CYAN "%-*s" ANSI_RESET, size, file->name);
	else if (S_ISFIFO(file->stats.st_mode))
		ft_printf(ANSI_YELLOW "%-*s" ANSI_RESET, size, file->name);
	else if (S_ISLNK(file->stats.st_mode))
		ft_printf(ANSI_PURPLE "%-*s" ANSI_RESET, size, file->name);
	else if ((((file->stats.st_mode) & S_IXUSR) == S_IXUSR))
		ft_printf(ANSI_RED "%-*s" ANSI_RESET, size, file->name);
	else
		ft_printf("%-*s", size, file->name);
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

void		print_full_info(t_list *files)
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
			ft_printf("%total %lld\n", get_totalsize(files));
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

void		simple_print_col(t_list *head)
{
	size_t	size;
	t_file	file;
	int		i;
	int		col;
	int		mod;
	t_list	*files;
	int		id;

	if (!head)
		return ;
	size = get_max_name_length(head) + 1;
	col = get_col(head);
	mod = 0;
	if (g_multiarg)
		print_path(((t_file *)head->content)->path);
	id = ((t_file *)head->content)->id;
	while (mod < col)
	{
		i = 0;
		files = head;
		while (files)
		{
			file = *((t_file*)files->content);
			if (i % col == mod)
			{
				// printf("file,id %d : file.name %s\n", file.id, file.name);
				if (size < file.size)
					size = file.size + 1;
				if (*(file.name) != '.' || (g_flags & F_DOT))
					print_name(&file, size);
			}
			if (*(file.name) != '.' || (g_flags & F_DOT))
				i++;
			files = files->next;
			if (files && ((t_file *)files->content)->id != id)
				break ;
		}
		mod++;
		if (mod < col)
			ft_putchar('\n');
	}
	ft_putchar('\n');
	if (files)
	{
		ft_putchar('\n');
		simple_print_col(files);
	}
}

void		simple_print(t_list *files)
{
	size_t	size;
	t_file	file;

	if (!files)
		return ;
	size = get_max_name_length(files) + 1;
	while (files)
	{
		file = *((t_file*)files->content);
		if (g_multiarg && (*(file.name) == '.') && !(*(file.name + 1)))
		{
			size = get_max_name_length(files) + 1;
			print_path(file.path);
		}
		if (size < file.size)
			size = file.size + 1;
		if (*(file.name) != '.' || (g_flags & F_DOT))
			print_name(&file, check_next(files->next, size));
		files = files->next;
	}
}

int			print_path(char *path)
{
	if (path)
	{
		ft_putstr(path);
		ft_putstr(":\n");
	}
	return (0);
}
