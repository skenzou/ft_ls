/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Mohamed <Mohamed@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 11:14:51 by Mohamed           #+#    #+#             */
/*   Updated: 2019/04/12 05:45:39 by midrissi         ###   ########.fr       */
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

int			sort_fiflnks(int argc, char **argv, char reverse)
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

int		sort_args_t(int argc, t_list *argv[argc], char reverse)
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
			if (argv[j + 1] && (ret = compare_folder_t(argv[j], argv[j + 1])))
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

void 		ft_listdel(t_list *head)
{
	t_file	*file;
	t_list	*temp;

	while (head)
	{
		file = (t_file *)head->content;
		ft_strdel(&(file->name));
		ft_strdel(&(file->pwd));
		ft_strdel(&(file->group));
		temp = head;
		head = head->next;
		free(temp);
	}
}
