/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: midrissi <midrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/09 01:44:57 by midrissi          #+#    #+#             */
/*   Updated: 2019/04/09 01:45:49 by midrissi         ###   ########.fr       */
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
