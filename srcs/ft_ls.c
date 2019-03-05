/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: midrissi <midrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 15:30:46 by midrissi          #+#    #+#             */
/*   Updated: 2019/03/05 06:56:20 by midrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void		list_dir(DIR *dir)
{
	t_dirent	*d;
	size_t		size;

	size = 10;
	while ((d = readdir(dir)))
	{
		if (size < ft_strlen(d->d_name))
			size = ft_strlen(d->d_name) + 1;
		if (*(d->d_name) != '.')
			ft_printf("%-*s", size, d->d_name);
	}
	ft_printf("\n");
	closedir(dir);
}

// void	perms(t_file *file, int mode)
// {
// 	file->perms[0] = 0;
// 	(S_ISREG(mode)) && (file->perms[0] = '-');
// 	(S_ISDIR(mode)) && (file->perms[0] = 'd');
// 	(S_ISBLK(mode)) && (file->perms[0] = 'b');
// 	(S_ISCHR(mode)) && (file->perms[0] = 'c');
// 	(S_ISFIFO(mode)) && (file->perms[0] = 'p');
// 	(S_ISLNK(mode)) && (file->perms[0] = 'l');
// 	(S_ISSOCK(mode)) && (file->perms[0] = 's');
// 	file->perms[0] || (file->perms[0] = '?');
// 	file->perms[1] = ((mode & S_IRUSR) ? 'r' : '-');
// 	file->perms[2] = ((mode & S_IWUSR) ? 'w' : '-');
// 	file->perms[3] = ((mode & S_IXUSR) ? 'x' : '-');
// 	file->perms[4] = ((mode & S_IRGRP) ? 'r' : '-');
// 	file->perms[5] = ((mode & S_IWGRP) ? 'w' : '-');
// 	file->perms[6] = ((mode & S_IXGRP) ? 'x' : '-');
// 	file->perms[7] = ((mode & S_IROTH) ? 'r' : '-');
// 	file->perms[8] = ((mode & S_IWOTH) ? 'w' : '-');
// 	file->perms[9] = ((mode & S_IXOTH) ? 'x' : '-');
// }

t_file		create_file(char *name)
{
	t_file		file;

	stat(name, &(file.stats));
	file.name = name;
	file.perms[0] = 0;
	(S_ISREG(file.stats.st_mode)) && (file.perms[0] = '-');
	(S_ISDIR(file.stats.st_mode)) && (file.perms[0] = 'd');
	(S_ISBLK(file.stats.st_mode)) && (file.perms[0] = 'b');
	(S_ISCHR(file.stats.st_mode)) && (file.perms[0] = 'c');
	(S_ISFIFO(file.stats.st_mode)) && (file.perms[0] = 'p');
	(S_ISLNK(file.stats.st_mode)) && (file.perms[0] = 'l');
	(S_ISSOCK(file.stats.st_mode)) && (file.perms[0] = 's');
	file.perms[0] || (file.perms[0] = '?');
	file.perms[1] = ((file.stats.st_mode & S_IRUSR) ? 'r' : '-');
	file.perms[2] = ((file.stats.st_mode & S_IWUSR) ? 'w' : '-');
	file.perms[3] = ((file.stats.st_mode & S_IXUSR) ? 'x' : '-');
	file.perms[4] = ((file.stats.st_mode & S_IRGRP) ? 'r' : '-');
	file.perms[5] = ((file.stats.st_mode & S_IWGRP) ? 'w' : '-');
	file.perms[6] = ((file.stats.st_mode & S_IXGRP) ? 'x' : '-');
	file.perms[7] = ((file.stats.st_mode & S_IROTH) ? 'r' : '-');
	file.perms[8] = ((file.stats.st_mode & S_IWOTH) ? 'w' : '-');
	file.perms[9] = ((file.stats.st_mode & S_IXOTH) ? 'x' : '-');
	return (file);
}

void		print_full_info(t_file file)
{
	ft_printf("%s %ld %s %s %lld %.12s %s\n",
	file.perms,
	file.stats.st_nlink,
	getpwuid(file.stats.st_uid)->pw_name,
	getgrgid(file.stats.st_gid)->gr_name,
	file.stats.st_size,
	ctime(&file.stats.st_mtimespec.tv_sec) + 4,
	file.name);
}

// int        main(int argc, char **argv)
// {
//     char arg = get_args(argc, argv);
//     ft_printf("arg vaut : %d\n", arg);
//     if (!arg)
//         ft_printf("Pas dargument/argument invalide\n");
//     else
//     {
//         (arg & F_LIST) && (ft_printf("flags: F_LIST\n"));
//         (arg & F_RECURSIVE) && (ft_printf("flags: F_RECURSIVE\n"));
//         (arg & F_DOT) && (ft_printf("flags: F_DOT\n"));
//         (arg & F_REVERSE) && (ft_printf("flags: F_REVERSE\n"));
//         (arg & F_SORT_TIME) && (ft_printf("flags: F_SORT TIME\n"));
//         (arg & F_LAST_ACCESS_TIME) && (ft_printf("flags: F_ACCESS TIME\n"));
//         (arg & F_SORT_OFF) && (ft_printf("flags: F_SORT OFF\n"));
//         (arg & F_COLOR) && (ft_printf("flags: F_COLOR\n"));
//     }
//     return (0);
// }

char    get_args(int argc, char **argv)
{
    int a;
    char flags;

    flags = 0;
    a = -1;
    while (++a < argc)
    {
        if (*argv[a] == '-' && (*argv[a]++))
        {
            while (*argv[a] != ' ' && *argv[a])
            {
                if (~ft_indexof(FLAGS, *argv[a]))
                    flags |= (1 << (ft_indexof(FLAGS, *argv[a]) - 1));
                (void)*argv[a]++;
            }
        }
    }
    return (flags);
}

int			main(int argc, char **argv)
{
	DIR			*dir;
	t_file		file;

	if (argc < 2)
		return (0);
	dir = opendir(argv[1]);
	list_dir(dir);
	file = create_file(argv[1]);
	print_full_info(file);
	return (0);
}
