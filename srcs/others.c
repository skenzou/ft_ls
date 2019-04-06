/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   others.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-azz <aben-azz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/06 12:38:49 by aben-azz          #+#    #+#             */
/*   Updated: 2019/04/06 12:48:50 by aben-azz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void		print_link(t_file *file)
{
	char	buff[MAX_PATH_LEN];

	ft_bzero(buff, MAX_PATH_LEN);
	if (readlink(file->full_path, buff, MAX_PATH_LEN) >= 0)
		ft_printf(" -> %s", buff);
	else
		exit(1);
}

int				check_next(t_list *list, int size)
{
	t_file file;

	if (!list)
		return (-1);
	file = *((t_file*)list->content);
	if (*(file.name) == '.' && !(*(file.name + 1)))
		return (-2);
	return (size);
}
