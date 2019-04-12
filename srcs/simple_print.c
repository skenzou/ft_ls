/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: midrissi <midrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/13 00:26:09 by midrissi          #+#    #+#             */
/*   Updated: 2019/04/13 01:05:41 by midrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void			print_name(t_file *file, int size)
{
	if (g_flags & F_COLOR)
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

int				continue_reading(t_list *head)
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

static void		simple_print_name(t_file *file, size_t size, int i, int col)
{
	if (i > file->nbfiles - col && i <= file->nbfiles)
		print_name(file, 0);
	else
		print_name(file, size);
}

static void		simple_print_loop(t_list *head, size_t size, int col)
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

void			simple_print(t_list *head, int header)
{
	size_t		size;

	if (!head)
		return ;
	size = get_max_name_length(head) + 1;
	print_head(((t_file *)head->content)->path, header, NULL);
	simple_print_loop(head, size, 0);
}
