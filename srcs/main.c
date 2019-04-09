/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: midrissi <midrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 15:30:46 by midrissi          #+#    #+#             */
/*   Updated: 2019/04/09 08:19:59 by Mohamed          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

char g_flags;
char g_multiarg;

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

static	void	ft_ls(int argc, char **names)
{
	DIR			*dir;
	t_list		*head;
	t_list		*fiflnks;
	int			i;

	head = NULL;
	fiflnks = NULL;
	i = argc == 1 ? 0 : 1;
	while (i < argc)
	{
		if (!(dir = opendir(names[i])))
			handle_notdir(names[i], &fiflnks);
		else
			list_dir(dir, &head, names[i]);
		i++;
	}
	if (!(g_flags & F_REVERSE))
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

int				main(int argc, char **argv)
{
	set_lsflags(argc, argv);
	(g_flags > 0) && (argv++);
	argc = argc - (g_flags > 0);
	(g_multiarg = argc > 2) && sort_args(argc, argv);
	if (argc == 1 && (argv[0] = "."))
		ft_ls(argc, argv);
	else
		ft_ls(argc, argv);
	return (0);
}
