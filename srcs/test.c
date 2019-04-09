/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Mohamed <Mohamed@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/09 07:14:00 by Mohamed           #+#    #+#             */
/*   Updated: 2019/04/09 07:22:47 by Mohamed          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int main(void)
{
	t_file file1;
	t_file file2;

	file1 = create_file("sbin", "/");
	file2 = create_file("bin", "/");
	printf("%ld\n", file1.stats.st_mtimespec.tv_sec -
					file2.stats.st_mtimespec.tv_sec);
	printf("%d\n", ft_strcmp(file1.full_path,
					file2.full_path));
}
