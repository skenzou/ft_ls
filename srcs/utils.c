/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-azz <aben-azz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/06 12:38:49 by aben-azz          #+#    #+#             */
/*   Updated: 2019/04/06 19:46:16 by aben-azz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int		print_link(t_file *file)
{
	char	buff[MAX_PATH_LEN];

	ft_bzero(buff, MAX_PATH_LEN);
	if (readlink(file->full_path, buff, MAX_PATH_LEN) >= 0)
		ft_printf(" -> %s", buff);
	else
		exit(1);
	return (0);
}

int		check_next(t_list *list, int size)
{
	t_file file;

	if (!list)
		return (-1);
	file = *((t_file*)list->content);
	if (*(file.name) == '.' && !(*(file.name + 1)))
		return (-2);
	return (size);
}

void		sort_list(t_list *list)
{
	void	*temp;
	t_list	*j;
	int		sorted;

	sorted = 0;
	while (!sorted)
	{
		j = list;
		sorted = 1;
		while (j)
		{
			if (j->next && ft_strcmp(((t_file *)j->content)->name,
									((t_file *)j->next->content)->name) > 0)
			{
				sorted = 0;
				temp = j->content;
				j->content = j->next->content;
				j->next->content = temp;
			}
			j = j->next;
		}
	}
}

void set_perms(t_file *file)
{
	file->perms[0] || (file->perms[0] = '?');
	file->perms[1] = "-r"[(file->stats.st_mode & S_IRUSR) > 0];
	file->perms[2] = "-w"[(file->stats.st_mode & S_IWUSR) > 0];
	file->perms[3] = third_permission(file->stats.st_mode, 'u');
	file->perms[4] = "-r"[(file->stats.st_mode & S_IRGRP) > 0];
	file->perms[5] = "-w"[(file->stats.st_mode & S_IWGRP) > 0];
	file->perms[6] = third_permission(file->stats.st_mode, 'g');
	file->perms[7] = "-r"[(file->stats.st_mode & S_IROTH) > 0];
	file->perms[8] = "-w"[(file->stats.st_mode & S_IWOTH) > 0];
	file->perms[9] = third_permission(file->stats.st_mode, 'o');
	file->perms[10] = get_extended(file);
	file->perms[11] = '\0';
}
