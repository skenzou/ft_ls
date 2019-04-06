/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   geters.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-azz <aben-azz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/06 12:27:31 by aben-azz          #+#    #+#             */
/*   Updated: 2019/04/06 14:15:02 by aben-azz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int			get_max_name_length(t_list *files)
{
	int		len;
	t_file	file;

	len = 0;
	while (files)
	{
		file = *((t_file*)files->content);
		if ((int)ft_strlen(file.name) > len)
			len = ft_strlen(file.name);
		files = files->next;
		if (g_multiarg && files)
		{
			file = *((t_file*)files->content);
			if (*(file.name) == '.' && !(*(file.name + 1)))
				break ;
		}
	}
	return (len);
}

int			get_col(t_list *files)
{
	int		nbfile;
	int		fileperline;
	t_file	file;

	fileperline = get_term_colsize() / (get_max_name_length(files) + 1);
	nbfile = 0;
	while (files)
	{
		file = *((t_file *)files->content);
		if (*(file.name) != '.' || (g_flags & F_DOT))
			nbfile++;
		files = files->next;
		if (g_multiarg && files)
		{
			file = *((t_file *)files->content);
			if (*(file.name) == '.' && !(*(file.name + 1)))
				break ;
		}
	}
	return ((nbfile / fileperline) + (nbfile % fileperline != 0 ? 1 : 0));
}

long long	get_totalsize(t_list *files)
{
	long long		size;
	t_file			file;

	size = 0;
	while (files)
	{
		file = *((t_file*)files->content);
		size += file.stats.st_blocks;
		files = files->next;
		if (g_multiarg && files)
		{
			file = *((t_file*)files->content);
			if (*(file.name) == '.' && !(*(file.name + 1)))
				break ;
		}
	}
	return (size);
}

char		get_extended(t_file file)
{
	(void)file;
	return ('@');
}

int			get_term_colsize(void)
{
	struct winsize w;

	ft_bzero((void *)&w, sizeof(w));
	if (ioctl(0, TIOCGWINSZ, &w) == -1)
		ft_printf("Error while retrieving terminal informations");
	return (w.ws_col);
}
