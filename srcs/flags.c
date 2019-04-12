/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flags.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: midrissi <midrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/13 00:39:34 by midrissi          #+#    #+#             */
/*   Updated: 2019/04/13 00:39:48 by midrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void		usage(char c)
{
	ft_putstr_fd("ft_ls: illegal option -- ", 2);
	write(2, &c, 1);
	ft_putendl_fd("\nusage: ft_ls [-lRarftuG] [file ...]", 2);
	exit(1);
}

char		**set_lsflags(int *argc, char **argv)
{
	int i;

	argv++ && (*argc)--;
	i = -1;
	while (++i < *argc)
	{
		if (*argv[i] != '-')
		{
			(*argc) -= i;
			return (argv + i);
		}
		else
			argv[i]++;
		while (*(argv[i]))
		{
			if (!ft_strchr(LSFLAGS, *(argv[i])))
				usage(*(argv[i]));
			g_flags |= (1 << (ft_indexof(LSFLAGS, *argv[i])));
			argv[i]++;
		}
	}
	*argc -= i;
	return (argv - 1);
}
