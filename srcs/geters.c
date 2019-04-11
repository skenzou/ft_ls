/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   geters.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-azz <aben-azz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/06 12:27:31 by aben-azz          #+#    #+#             */
/*   Updated: 2019/04/11 08:05:39 by Mohamed          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int			get_max_name_length(t_list *files)
{
	int		len;
	t_file	file;
	int		id;

	len = 0;
	id = ((t_file *)files->content)->id;
	while (files)
	{
		file = *((t_file *)files->content);
		if (*(file.name) != '.' || (g_flags & F_DOT))
			len = ft_max(file.size, len);
		files = files->next;
		if (g_multiarg && files)
		{
			file = *((t_file *)files->content);
			if (file.id != id)
				break ;
		}
	}
	return (len);
}

int			get_col(t_list *files, int *nbfile)
{
	int		fileperline;
	t_file	file;
	int		id;
	int		col;

	fileperline = get_term_colsize() / (get_max_name_length(files) + 1);
	*nbfile = 0;
	id = ((t_file *)files->content)->id;
	while (files)
	{
		file = *((t_file *)files->content);
		if (*(file.name) != '.' || (g_flags & F_DOT))
			(*nbfile)++;
		files = files->next;
		if (g_multiarg && files)
		{
			file = *((t_file *)files->content);
			if (file.id != id)
				break ;
		}
	}
	col = (*nbfile / fileperline) + (*nbfile % fileperline != 0 ? 1 : 0);
	return (col > 0 ? col : 1);
}

long long	get_totalsize(t_list *files)
{
	long long		size;
	t_file			file;
	int				id;
	int				found;

	if (!files)
		return (0);
	size = 0;
	id = ((t_file*)files->content)->id;
	found = 0;
	while (files)
	{
		file = *((t_file*)files->content);
		if (*(file.name) != '.' || (g_flags & F_DOT))
			found = 1;
		size += file.stats.st_blocks;
		files = files->next;
		if (g_multiarg && files)
		{
			file = *((t_file*)files->content);
			if (file.id != id)
				break ;
		}
	}
	return (found ? size : -1);
}

char		get_extended(t_file *file)
{
	ssize_t		attr;
	acl_t		acl;
	acl_entry_t dummy;

	acl = NULL;
	attr = listxattr(file->full_path, NULL, 0, XATTR_NOFOLLOW);
	acl = acl_get_link_np(file->full_path, ACL_TYPE_EXTENDED);
	if (acl && acl_get_entry(acl, ACL_FIRST_ENTRY, &dummy) == -1)
	{
		acl_free(acl);
		acl = NULL;
	}
	if (attr > 0)
		return ('@');
	else if (acl != 0)
		return ('+');
	else
		return (' ');
}

int			get_term_colsize(void)
{
	struct winsize w;

	ft_bzero((void *)&w, sizeof(w));
	if (ioctl(0, TIOCGWINSZ, &w) == -1)
		ft_printf("Error while retrieving terminal informations");
	return (w.ws_col);
}
