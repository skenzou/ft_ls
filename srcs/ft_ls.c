/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: midrissi <midrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 15:30:46 by midrissi          #+#    #+#             */
/*   Updated: 2019/03/01 22:42:40 by midrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void    list_dir(DIR *dir)
{
   t_dirent *d;
   size_t size;

   size = 10;
  while ((d = readdir(dir)))
  {
    if (size < ft_strlen(d->d_name))
      size = ft_strlen(d->d_name) + 1;
    if (*(d->d_name) != '.')
      ft_printf("%-*s",size, d->d_name);
  }
  ft_printf("\n");
  closedir(dir);
}

t_file  *create_file(char *name)
{
  t_file  *file;

  file = (t_file *)malloc(sizeof(t_file));
  stat(name, &(file->stats));
  file->name = name;
  return(file);
}

int main(int argc, char **argv)
{
  DIR           *dir;
  t_file        *file;

  file = NULL;
  if (argc < 2)
    return (0);
  dir = opendir(argv[1]);
  list_dir(dir);
  file = create_file(argv[1]);
  printf("test: %s\n", getpwuid(file->stats.st_uid)->pw_name);
  printf("test: %s\n", getgrgid(file->stats.st_gid)->gr_name);
  printf("test: %s\n", file->name);
  return (0);
}
