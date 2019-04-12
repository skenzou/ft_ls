/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: midrissi <midrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 15:30:46 by midrissi          #+#    #+#             */
/*   Updated: 2019/04/13 01:03:37 by midrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

char g_flags;

void			ft_ls_r(t_file *file)
{
	DIR			*dir;
	t_list		*head;

	ft_putchar('\n');
	head = NULL;
	if ((dir = opendir(file->full_path)))
		list_dir(dir, &head, file->full_path);
	else
	{
		ft_putstr(file->full_path);
		ft_putstr(":\n");
		print_err(file->name);
		return ;
	}
	if (!g_flags || (~g_flags & F_LIST))
		simple_print(head, 1);
	else
		print_full_info(head, 1);
	ft_listdel(head);
}

static int		check_dirlnk(char *name, t_list **fiflnks)
{
	t_file	file;
	t_list	*list;

	if (!(g_flags & F_LIST))
		return (1);
	file = create_file(name, NULL, 0, 0);
	if (S_ISLNK(file.stats.st_mode))
	{
		file.namesize = ft_strlen(name);
		list = ft_lstnew((void *)&file, sizeof(t_file));
		list == NULL ? exit(1) : 0;
		if (g_flags & F_LAST_MODIF)
			(g_flags & F_REVERSE) ? insert_time_r(fiflnks, list, 1)
												: insert_time(fiflnks, list, 1);
		else
			ft_lstadd(fiflnks, list);
		return (0);
	}
	return (1);
}

static void		parse_biglist(int argc, t_list *biglist[], int i)
{
	int j;

	j = 1;
	while (j < argc)
	{
		if (!g_flags || (~g_flags & F_LIST))
			simple_print(biglist[j], i > 1);
		else
			print_full_info(biglist[j], i > 1);
		ft_listdel(biglist[j++]);
		if (j < argc)
			ft_putchar('\n');
	}
	ft_listdel(biglist[0]);
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
		else if (check_dirlnk(names[i], &biglist[0]))
			list_dir(dir, &biglist[j++], names[i]);
	!(g_flags & F_LAST_MODIF) ? ft_lstrev(&biglist[0]) : 0;
	handle_fiflnks(biglist[0], biglist[1]);
	if (g_flags & F_LAST_MODIF)
		sort_args_time(j, biglist, (g_flags & F_REVERSE));
	parse_biglist(j, biglist, i);
}

int				main(int argc, char **argv)
{
	argv = set_lsflags(&argc, argv);
	if (argc == 0 && (argv[0] = "."))
		ft_ls(argc + 1, argv);
	else
	{
		(argc > 1) && sort_args(argc, argv, (g_flags & F_REVERSE));
		ft_ls(argc, argv);
	}
	return (0);
}
