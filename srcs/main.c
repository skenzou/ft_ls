/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: midrissi <midrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 15:30:46 by midrissi          #+#    #+#             */
/*   Updated: 2019/04/12 00:03:18 by aben-azz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

char g_flags;
//
// void			print_flags(void)
// {
// 	(g_flags & F_LIST) && (ft_printf("g_flags: F_LIST\n"));
// 	(g_flags & F_RECURSIVE) && (ft_printf("g_flags: F_RECURSIVE\n"));
// 	(g_flags & F_DOT) && (ft_printf("g_flags: F_DOT\n"));
// 	(g_flags & F_REVERSE) && (ft_printf("g_flags: F_REVERSE\n"));
// 	(g_flags & F_SORT_TIME) && (ft_printf("g_flags: F_SORT TIME\n"));
// 	(g_flags & F_LAST_ACCESS) && (ft_printf("g_flags: F_ACCESS TIME\n"));
// 	(g_flags & F_SORT_OFF) && (ft_printf("g_flags: F_SORT OFF\n"));
// 	(g_flags & F_COLOR) && (ft_printf("g_flags: F_COLOR\n"));
// }

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
	{
		print_err(file->name);
		return ;
	}
	if (!g_flags || (~g_flags & F_LIST))
		simple_print_col(head, 1);
	else
		print_full_info(head, 1);
	ft_listdel(head);
}

static	void	ft_ls(int argc, char **names)
{
	DIR			*dir;
	int			i;
	t_list		*biglist[argc + 1];
	int			j;

	ft_bzero((void *)&biglist, sizeof(biglist));
	i = -1;
	j = 1;
	while (++i < argc)
		if (!(dir = opendir(names[i])))
			handle_notdir(names[i], &biglist[0]);
		else
			list_dir(dir, &biglist[j++], names[i]);
	ft_lstrev(&biglist[0]);
	handle_fiflnks(biglist[0], biglist[1]);
	(g_flags & F_LAST_ACCESS) && sort_args_t(j, biglist, (g_flags & F_REVERSE));
	argc = j;
	j = 1;
	while (j < argc)
	{
		if (!g_flags || (~g_flags & F_LIST))
			simple_print_col(biglist[j], i > 1);
		else
			print_full_info(biglist[j], i > 1);
		ft_listdel(biglist[j++]);
		if (j < argc)
			ft_putchar('\n');
	}
	ft_listdel(biglist[0]);
}

int				main(int argc, char **argv)
{
	set_lsflags(argc, argv);
	//print_flags();
	(g_flags > 0) && (argv++);
	argc -= (g_flags > 0);
	if (argc == 1 && (argv[0] = "."))
		ft_ls(argc, argv);
	else
	{
		argv++ && argc--;
		(argc > 1) && sort_args(argc, argv, (g_flags & F_REVERSE));
		ft_ls(argc, argv);
	}
	return (0);
}
