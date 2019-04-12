/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: midrissi <midrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/09 01:44:57 by midrissi          #+#    #+#             */
/*   Updated: 2019/04/12 01:56:11 by midrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void        print_time(t_file *file)
{
    time_t        now;
    long long    diff;
    char        *date;

    time(&now);
    diff = now - file->stats.st_mtimespec.tv_sec;
    date = ctime(&file->stats.st_mtimespec.tv_sec);
    date == NULL ? exit(1) : 0;
    if (diff > SIX_MONTHS || diff < 0)
    {
        ft_putchar(' ');
        write(1, date + 4, 7);
        ft_putchar(' ');
        write(1, date + 20, 4);
				ft_putchar(' ');
    }
    else
    {
        ft_putchar(' ');
        write(1, date + 4, 12);
        ft_putchar(' ');
    }
}

int			lsprint(char *string, int namesize, int padding, char *color)
{
	write(1, color, ft_strlen(color));
	write(1, string, namesize);
	write(1, "\x1b[0m", 4);
	ft_nputchar(' ', ft_max(0, padding - namesize));
	return (0);
}

int lsputnbr(int nbr, int padding, int afterspace)
{
	ft_putchar(' ');
	ft_nputchar(' ', padding - ft_intlen_base(nbr, 10));
	ft_putnbr(nbr);
	if (afterspace)
		ft_putchar(' ');
	return (1);
}

int				print_link(t_file *file)
{
	char	buff[MAX_PATH_LEN];

	ft_bzero(buff, MAX_PATH_LEN);
	if (readlink(file->full_path, buff, MAX_PATH_LEN) >= 0)
	{
		ft_putstr(" -> ");
		ft_putstr(buff);
	}
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
	{
		ft_putstr("total ");
		ft_putnbr(totalsize);
		ft_putchar('\n');
	}
	return (1);
}
