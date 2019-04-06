/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: midrissi <midrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 15:30:46 by midrissi          #+#    #+#             */
/*   Updated: 2019/04/06 03:33:42 by midrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
#include <sys/ioctl.h>
char g_flags = 0;
char g_multiarg = 0;

void			print_path(char *path)
{
	if (path)
	{
		ft_putstr(path);
		ft_putstr(":\n");
	}
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

void			print_name(t_file *file, int size)
{
	if (S_ISDIR(file->stats.st_mode))
		ft_printf(ANSI_CYAN "%-*s" ANSI_RESET, size, file->name);
	else if (S_ISFIFO(file->stats.st_mode))
		ft_printf(ANSI_YELLOW "%-*s" ANSI_RESET, size, file->name);
	else if (S_ISLNK(file->stats.st_mode))
		ft_printf(ANSI_PURPLE "%-*s" ANSI_RESET, size, file->name);
	else if ((((file->stats.st_mode) & S_IXUSR) == S_IXUSR))
		ft_printf(ANSI_RED "%-*s" ANSI_RESET, size, file->name);
	else
		ft_printf("%-*s", size, file->name);
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

int		get_max_name_length(t_list *files)
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

int			get_term_colsize(void)
{
	struct winsize w;

	ft_bzero((void *)&w, sizeof(w));
	if (ioctl(0, TIOCGWINSZ, &w) == -1)
		ft_printf("Error while retrieving terminal informations");
	return (w.ws_col);
}

int					get_col(t_list *files)
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

void 	print_list(t_list *files)
{
	t_file file;

	printf("====================================================\n");
	while (files)
	{
		file = *((t_file *)files->content);
		printf("files.id %d -> %s\n", file.id, file.full_path);
		files = files->next;
	}
	printf("====================================================\n");
}

static void		simple_print_col(t_list *head)
{
	size_t	size;
	t_file	file;
	int		i;
	int		col;
	int		mod;
	t_list	*files;
	int		id;

	if (!head)
		return ;
	size = get_max_name_length(head) + 1;
	col = get_col(head);
	mod = 0;
	if (g_multiarg)
		print_path(((t_file *)head->content)->path);
	id = ((t_file *)head->content)->id;
	while (mod < col)
	{
		i = 0;
		files = head;
		while (files)
		{
			file = *((t_file*)files->content);
			if (i % col == mod)
			{
				// printf("file,id %d : file.name %s\n", file.id, file.name);
				if (size < file.size)
					size = file.size + 1;
				if (*(file.name) != '.' || (g_flags & F_DOT))
					print_name(&file, size);
			}
			if (*(file.name) != '.' || (g_flags & F_DOT))
				i++;
			files = files->next;
			if (files && ((t_file *)files->content)->id != id)
				break ;
		}
		mod++;
		if (mod < col)
			ft_putchar('\n');
	}
	ft_putchar('\n');
	if (files)
	{
		ft_putchar('\n');
		simple_print_col(files);
	}
}

static void		simple_print(t_list *files)
{
	size_t	size;
	t_file	file;

	if (!files)
		return ;
	size = get_max_name_length(files) + 1;
	while (files)
	{
		file = *((t_file*)files->content);
		if (g_multiarg && (*(file.name) == '.') && !(*(file.name + 1)))
		{
			size = get_max_name_length(files) + 1;
			print_path(file.path);
		}
		if (size < file.size)
			size = file.size + 1;
		if (*(file.name) != '.' || (g_flags & F_DOT))
			print_name(&file, check_next(files->next, size));
		files = files->next;
	}
}

char		get_extended(t_file file)
{
	(void)file;
	return ('@');
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

void		list_insert(t_list **head, t_list **tail, t_list *needle)
{
	t_list *curr;
	t_list *prev;

	if (!tail || !head || !needle)
		return ;
	if (!(*head))
	{
		ft_lstadd(head, needle);
		*tail = *head;
		return ;
	}
	if (ft_strcmp(((t_file *)(*head)->content)->full_path, ((t_file *)needle->content)->full_path) >= 0)
	{
		ft_lstadd(head, needle);
		return ;
	}
	if (ft_strcmp(((t_file *)(*tail)->content)->full_path, ((t_file *)needle->content)->full_path) <= 0)
	{
		(*tail)->next = needle;
		*tail = needle;
		return ;
	}
	curr = (*head)->next;
	prev = *head;
	while (curr)
	{
		if (ft_strcmp(((t_file *)(curr)->content)->full_path, ((t_file *)needle->content)->full_path) >= 0)
		{
			prev->next = needle;
			needle->next = curr;
			return ;
		}
		curr = curr->next;
		prev = prev->next;
	}
}

char		third_permission(int m, char type_user)
{
	if (type_user == 'u')
		return (char[2][2]){"-S", "xs"}[(m & S_IXUSR) > 0][(m & S_ISUID) > 0];
	else if (type_user == 'g')
		return (char[2][2]){"-S", "xs"}[(m & S_IXGRP) > 0][(m & S_ISGID) > 0];
	else
		return ((char[2][2]){"-T", "xt"}[(m & S_IXOTH) > 0][(m & S_ISTXT) > 0]);
}

static void		cat_fullpath(t_file *file, char *name, char *path)
{
	ft_bzero(file->full_path, MAX_PATH_LEN);
	if (path)
	{
		ft_strcpy(file->full_path, path);
		ft_strcat(file->full_path, "/");
	}
	ft_strcat(file->full_path, name);
	file->name = ft_strdup(name);
	!file->name ? exit(1) : 0;
	file->path = path;
}

t_file			create_file(char *name, char *path)
{
	t_file		file;

	ft_bzero((void *)&file, sizeof(t_file));
	cat_fullpath(&file, name, path);
	lstat(file.full_path, &(file.stats));
	file.perms[0] = 0;
	(S_ISREG(file.stats.st_mode)) && (file.perms[0] = '-');
	(S_ISDIR(file.stats.st_mode)) && (file.perms[0] = 'd');
	(S_ISBLK(file.stats.st_mode)) && (file.perms[0] = 'b');
	(S_ISCHR(file.stats.st_mode)) && (file.perms[0] = 'c');
	(S_ISFIFO(file.stats.st_mode)) && (file.perms[0] = 'p');
	(S_ISLNK(file.stats.st_mode)) && (file.perms[0] = 'l');
	(S_ISSOCK(file.stats.st_mode)) && (file.perms[0] = 's');
	file.perms[0] || (file.perms[0] = '?');
	file.perms[1] = "-r"[(file.stats.st_mode & S_IRUSR) > 0];
	file.perms[2] = "-w"[(file.stats.st_mode & S_IWUSR) > 0];
	file.perms[3] = third_permission(file.stats.st_mode, 'u');
	file.perms[4] = "-r"[(file.stats.st_mode & S_IRGRP) > 0];
	file.perms[5] = "-w"[(file.stats.st_mode & S_IWGRP) > 0];
	file.perms[6] = third_permission(file.stats.st_mode, 'g');
	file.perms[7] = "-r"[(file.stats.st_mode & S_IROTH) > 0];
	file.perms[8] = "-w"[(file.stats.st_mode & S_IWOTH) > 0];
	file.perms[9] = third_permission(file.stats.st_mode, 'o');
	//file.perms[10] = get_extended(file);
	file.perms[10] = ' ';
	file.perms[11] = '\0';
	return (file);
}

void		list_dir(DIR *dir, t_list **head, t_list **tail, char *path, int i)
{
	t_dirent	*d;
	t_list		*list;
	t_file		file;

	list = NULL;
	while ((d = readdir(dir)))
	{
		file = create_file(d->d_name, path);
		file.size = ft_strlen(d->d_name);
		file.id = i;
		list = ft_lstnew((void *)&file, sizeof(t_file));
		list == NULL ? exit(1) : 0;
		list_insert(head, tail, list);
	}
	closedir(dir);
}

int	ft_intlen_base(uintmax_t nbr, int base)
{
	int digits;

	digits = 0;
	while (nbr)
	{
		digits++;
		nbr /= base;
	}
	return (digits);
}

static void		set_max_length(t_list *files, int length[4])
{
	t_file	f;

	length[0] = 0;
	length[1] = 0;
	length[2] = 0;
	length[3] = 0;
	while (files)
	{
		f = *((t_file *)files->content);
		if (ft_intlen_base(f.stats.st_nlink, 10) > length[0])
			length[0] = ft_intlen_base(f.stats.st_nlink, 10);
		if (ft_intlen_base(f.stats.st_size, 10) > length[1])
			length[1] = ft_intlen_base(f.stats.st_size, 10);
		if ((int)ft_strlen(getpwuid(f.stats.st_uid)->pw_name) > length[2])
			length[2] = (int)ft_strlen(getpwuid(f.stats.st_uid)->pw_name);
		if ((int)ft_strlen(getgrgid(f.stats.st_gid)->gr_name) > length[3])
			length[3] = (int)ft_strlen(getgrgid(f.stats.st_gid)->gr_name);
		files = files->next;
	}
}

long long		get_totalsize(t_list *files)
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
static void		print_link(t_file *file)
{
	char	buff[MAX_PATH_LEN];

	ft_bzero(buff, MAX_PATH_LEN);
	if (readlink(file->full_path, buff, MAX_PATH_LEN) >= 0)
		ft_printf(" -> %s", buff);
	else
		exit(1);
}

static void		print_full_info(t_list *files)
{
	t_file	file;
	int		length[4];

	set_max_length(files, length);
	while (files)
	{
		file = *((t_file *)files->content);
		if (*(file.name) == '.' && !(*(file.name + 1)))
		{
			if (g_multiarg)
				print_path(file.path);
			ft_printf("%total %lld\n", get_totalsize(files));
		}
		if (*(file.name) != '.' || (g_flags & F_DOT))
		{
			ft_printf("%s %*hu %-*s %-*s %*lld %.12s ", file.perms, length[0],
			file.stats.st_nlink, length[2] + 1,
			getpwuid(file.stats.st_uid)->pw_name, length[3] + 1,
			getgrgid(file.stats.st_gid)->gr_name, length[1], file.stats.st_size,
			ctime(&file.stats.st_mtimespec.tv_sec) + 4);
			print_name(&file, check_next(files->next, 0));
			if (S_ISLNK(file.stats.st_mode))
				print_link(&file);
			ft_putchar('\n');
		}
		files = files->next;
	}
}

void			print_flags(void)
{
	(g_flags & F_LIST) && (ft_printf("g_flags: F_LIST\n"));
	(g_flags & F_RECURSIVE) && (ft_printf("g_flags: F_RECURSIVE\n"));
	(g_flags & F_DOT) && (ft_printf("g_flags: F_DOT\n"));
	(g_flags & F_REVERSE) && (ft_printf("g_flags: F_REVERSE\n"));
	(g_flags & F_SORT_TIME) && (ft_printf("g_flags: F_SORT TIME\n"));
	(g_flags & F_LAST_ACCESS_TIME) && (ft_printf("g_flags: F_ACCESS TIME\n"));
	(g_flags & F_SORT_OFF) && (ft_printf("g_flags: F_SORT OFF\n"));
	(g_flags & F_COLOR) && (ft_printf("g_flags: F_COLOR\n"));
}

static void			set_lsflags(int argc, char **argv)
{
	int i;

	i = -1;
	g_flags = 0;
	while (++i < argc)
		if (*argv[i] == '-' && argv[i]++)
			while (*(argv[i]) && *(argv[i]) != ' ')
			{
				if (ft_indexof(LSFLAGS, *argv[i]) >= 0)
					g_flags |= (1 << (ft_indexof(LSFLAGS, *argv[i])));
				argv[i]++;
			}
}

void				handle_notdir(char *name, t_list **fiflnks)
{
	char	*err;
	t_file	file;
	t_list	*list;

	ft_bzero((void *)&file, sizeof(t_file));
	file = create_file(name, NULL);
	if ((S_ISFIFO(file.stats.st_mode) || S_ISLNK(file.stats.st_mode)))
	{
		list = ft_lstnew((void *)&file, sizeof(t_file));
		list == NULL ? exit(1) : 0;
		ft_lstadd(fiflnks, list);
	}
	else
	{
		err = ft_strjoin("ls: ", name);
		perror(err);
		ft_strdel(&err);
	}
}

static void			handle_fiflnks(t_list *fiflnks, t_list *head)
{
	if (fiflnks)
	{
		if (!(g_flags & F_DOT))
		{
			g_flags |= F_DOT;
			if (!g_flags || (~g_flags & F_LIST))
				simple_print(fiflnks);
			else
				print_full_info(fiflnks);
			g_flags &= ~F_DOT;
		}
		else
		{
			if (!g_flags || (~g_flags & F_LIST))
				simple_print(fiflnks);
			else
				print_full_info(fiflnks);
		}
		if (head)
			ft_putchar('\n');
	}
}

static	void		ft_ls(int argc, char **names)
{
	DIR			*dir;
	t_list		*head;
	t_list		*tail;
	t_list		*fiflnks;
	int			i;

	head = NULL;
	tail = NULL;
	fiflnks = NULL;
	i = argc == 1 ? 0 : 1;
	while (i < argc)
	{
		dir = opendir(names[i]);
		if (!dir)
			handle_notdir(names[i], &fiflnks);
		else
			list_dir(dir, &head, &tail, names[i], i);
		i++;
	}
	ft_lstrev(&fiflnks);
	handle_fiflnks(fiflnks, head);
	if (!g_flags || (~g_flags & F_LIST))
		simple_print_col(head);
	else
		print_full_info(head);
}

static	int		sort_args(int argc, char **argv)
{
	int		sorted;
	int		j;
	char	*temp;

	sorted = 0;
	while (!sorted)
	{
		j = 1;
		sorted = 1;
		while (j < argc)
		{
			if (argv[j + 1] && ft_strcmp(argv[j], argv[j + 1]) > 0)
			{
				sorted = 0;
				temp = argv[j];
				argv[j] = argv[j + 1];
				argv[j + 1] = temp;
			}
			j++;
		}
	}
	return (1);
}

int					main(int argc, char **argv)
{
	set_lsflags(argc, argv);
	(g_flags > 0) && (argv++);
	argc = argc - (g_flags > 0);
	(g_multiarg = argc > 2) && sort_args(argc, argv);
	if (argc == 1 && (argv[0] = "."))
		ft_ls(argc, argv);
	else
		ft_ls(argc, argv);
	// print_flags();
	return (0);
}
