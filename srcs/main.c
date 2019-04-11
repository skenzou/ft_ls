/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: midrissi <midrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 15:30:46 by midrissi          #+#    #+#             */
/*   Updated: 2019/04/11 11:47:36 by Mohamed          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

char g_flags;

/*
**void			print_flags(void)
**{
**	(g_flags & F_LIST) && (ft_printf("g_flags: F_LIST\n"));
**	(g_flags & F_RECURSIVE) && (ft_printf("g_flags: F_RECURSIVE\n"));
**	(g_flags & F_DOT) && (ft_printf("g_flags: F_DOT\n"));
**	(g_flags & F_REVERSE) && (ft_printf("g_flags: F_REVERSE\n"));
**	(g_flags & F_SORT_TIME) && (ft_printf("g_flags: F_SORT TIME\n"));
**	(g_flags & F_LAST_ACCESS_TIME) && (ft_printf("g_flags: F_ACCESS TIME\n"));
**	(g_flags & F_SORT_OFF) && (ft_printf("g_flags: F_SORT OFF\n"));
**	(g_flags & F_COLOR) && (ft_printf("g_flags: F_COLOR\n"));
**}
**void 			print_list(t_list *files)
**{
**	t_file file;
**	printf("====================================================\n");
**	while (files)
**	{
**		file = *((t_file *)files->content);
**		printf("files.id %d -> %s\n", file.id, file.full_path);
**		files = files->next;
**	}
**	printf("====================================================\n");
**}
*/

static void		set_lsflags(int argc, char **argv)
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

void	ft_ls_r(t_file *file)
{
	DIR			*dir;
	t_list		*head;

	ft_putchar('\n');
	head = NULL;
	if ((dir = opendir(file->full_path)))
		list_dir(dir, &head, file->full_path);
	else
		print_err(file->name);
	if (!g_flags || (~g_flags & F_LIST))
		simple_print_col(head);
	else
		print_full_info(head);
	ft_listdel(head);
}

static	void	ft_ls(int argc, char **names)
{
	DIR			*dir;
	t_list		*head;
	t_list		*fiflnks;
	int			i;
	t_list		*biglist[argc];
	int			j;

	head = NULL;
	fiflnks = NULL;
	i = argc == 1 ? 0 : 1;
	j = 0;
	while (i < argc)
	{
		if (!(dir = opendir(names[i])))
			handle_notdir(names[i], &fiflnks);
		else
		{
			list_dir(dir, &head, names[i]);
			biglist[j++] = head;
			head = NULL;
		}
		i++;
	}
	biglist[j] = NULL;
	if (!(g_flags & F_REVERSE))
		ft_lstrev(&fiflnks);
	handle_fiflnks(fiflnks, head);
	ft_listdel(fiflnks);
	if ((g_flags & F_LAST_ACCESS_TIME))
		sort_args_time(j, biglist, (g_flags & F_REVERSE));
	i = 0;
	while (i < j)
	{
		if (!g_flags || (~g_flags & F_LIST))
			simple_print_col(biglist[i]);
		else
			print_full_info(biglist[i]);
		ft_listdel(biglist[i++]);
		if (i < j)
			ft_putchar('\n');
	}
}

int				main(int argc, char **argv)
{
	set_lsflags(argc, argv);
	(g_flags > 0) && (argv++);
	argc = argc - (g_flags > 0);
	(argc > 2) && sort_args(argc, argv, (g_flags & F_REVERSE));
	argc == 1 && (argv[0] = ".");
	ft_ls(argc, argv);
	return (0);
}
