/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Mohamed <Mohamed@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 11:14:51 by Mohamed           #+#    #+#             */
/*   Updated: 2019/04/13 00:45:16 by midrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int			sort_args(int argc, char **argv, char reverse)
{
	int		sorted;
	int		j;
	char	*temp;
	int		ret;

	sorted = 0;
	while (!sorted)
	{
		j = -1;
		sorted = 1;
		while (++j < argc)
			if (argv[j + 1] && (ret = ft_strcmp(argv[j], argv[j + 1])))
				if ((!reverse && ret > 0) || (reverse && ret < 0))
				{
					sorted = 0;
					temp = argv[j];
					argv[j] = argv[j + 1];
					argv[j + 1] = temp;
				}
	}
	return (1);
}

int			sort_args_time(int argc, t_list *argv[argc], char reverse)
{
	int		sorted;
	int		j;
	int		ret;
	t_list	*temp;

	sorted = 0;
	while (!sorted)
	{
		j = 0;
		sorted = 1;
		while (++j < argc - 1)
			if (argv[j + 1] &&
							(ret = compare_folder_time(argv[j], argv[j + 1])))
				if ((!reverse && ret < 0) || (reverse && ret > 0))
				{
					sorted = 0;
					temp = argv[j];
					argv[j] = argv[j + 1];
					argv[j + 1] = temp;
				}
	}
	return (1);
}

int			compare_folder_time(t_list *first, t_list *second)
{
	int ret;

	ret = ((t_file *)first->content)->time -
					((t_file *)second->content)->time;
	if (!ret)
		ret = ((t_file *)first->content)->ntime -
					((t_file *)second->content)->ntime;
	if (!ret)
		ret = ft_strcmp(((t_file *)second->content)->full_path,
					((t_file *)first->content)->full_path);
	return (ret);
}

void		print_err(char *name)
{
	char *err;

	err = ft_strjoin("ls: ", name);
	perror(err);
	ft_strdel(&err);
}

void		ft_listdel(t_list *head)
{
	t_file	*file;
	t_list	*temp;

	while (head)
	{
		file = (t_file *)head->content;
		ft_strdel(&(file->name));
		ft_strdel(&(file->pwd));
		ft_strdel(&(file->group));
		free(head->content);
		temp = head;
		head = head->next;
		free(temp);
	}
}
