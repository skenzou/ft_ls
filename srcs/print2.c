/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: midrissi <midrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/09 01:44:57 by midrissi          #+#    #+#             */
/*   Updated: 2019/04/09 04:19:17 by midrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void		print_newline(int size)
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

void		print_time(t_file *file)
{
	time_t		now;
	long long	diff;
	char		*date;

	time(&now);
	diff = now - file->stats.st_mtimespec.tv_sec;
	date = ctime(&file->stats.st_mtimespec.tv_sec);
	date == NULL ? exit(1) : 0;
	if (diff > SIX_MONTHS || diff < 0)
		ft_printf(" %.7s %.4s ", date + 4, date + 20);
	else
		ft_printf(" %.12s ", date + 4);
}
