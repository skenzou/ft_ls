/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: midrissi <midrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/09 01:44:57 by midrissi          #+#    #+#             */
/*   Updated: 2019/04/11 13:58:51 by Mohamed          ###   ########.fr       */
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

int				print_link(t_file *file)
{
	char	buff[MAX_PATH_LEN];

	ft_bzero(buff, MAX_PATH_LEN);
	if (readlink(file->full_path, buff, MAX_PATH_LEN) >= 0)
		ft_printf(" -> %s", buff);
	else
		exit(1);
	return (0);
}

int				print_head(char *path, int header, t_list *files)
{
	int		totalsize;

	if (header == -1)
		return (1);
	if (files)
		totalsize = get_totalsize(files);
	if (path && header)
	{
		ft_putstr(path);
		ft_putstr(":\n");
	}
	if ((g_flags & F_LIST) && totalsize != -1)
		ft_printf("total %lld\n", totalsize);
	return (1);
}
