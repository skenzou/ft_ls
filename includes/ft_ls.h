/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: midrissi <midrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 15:31:17 by midrissi          #+#    #+#             */
/*   Updated: 2019/04/06 14:14:33 by aben-azz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H

# include "../libft/includes/libft.h"
# include <stdio.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <pwd.h>
# include <grp.h>
# include <uuid/uuid.h>
# include <fcntl.h>
# include <dirent.h>
# include <time.h>
# include <sys/ioctl.h>
# define LSFLAGS			"lRarftuG"
# define F_LIST				(1 << 0)
# define F_RECURSIVE		(1 << 1)
# define F_DOT				(1 << 2)
# define F_REVERSE			(1 << 3)
# define F_SORT_TIME        (1 << 4)
# define F_LAST_ACCESS_TIME	(1 << 5)
# define F_SORT_OFF			(1 << 6)
# define F_COLOR			(1 << 7)
# define ANSI_RED			"\x1b[31m"
# define ANSI_GREEN			"\x1b[1m\x1b[32m"
# define ANSI_YELLOW		"\x1b[33m"
# define ANSI_BLUE			"\x1b[1m\x1b[34m"
# define ANSI_MAGENTA		"\x1b[1m\x1b[35m"
# define ANSI_CYAN			"\x1b[1m\x1b[36m"
# define ANSI_RESET			"\x1b[0m"
# define ANSI_PURPLE		"\x1b[35m"
# define MAX_PATH_LEN		4096
# define COLOR				0

t_list						*g_files;
extern char					g_flags;
extern char					g_multiarg;

typedef struct stat			t_stat;
typedef struct dirent		t_dirent;
typedef struct passwd		t_passwd;
typedef struct group		t_group;

typedef struct				s_file
{
	t_stat					stats;
	size_t					size;
	char					perms[11 + 1];
	char					*name;
	char					*path;
	char					full_path[MAX_PATH_LEN];
	char					printed;
	int						id;
}							t_file;
int							print_link(t_file *file);
int							check_next(t_list *list, int size);
int							get_max_name_length(t_list *files);
int							get_col(t_list *files);
long long					get_totalsize(t_list *files);
char						get_extended(t_file file);
int							get_term_colsize(void);
void						handle_notdir(char *name, t_list **fiflnks);
void						handle_fiflnks(t_list *fiflnks, t_list *head);
void						set_max_length(t_list *files, int len[4]);
char						third_permission(int m, char type_user);
void						cat_fullpath(t_file *file, char *name, char *path);
void						sort_list(t_list *list);
void						list_insert(t_list **h, t_list **t, t_list *n);
void						list_dir(DIR *d, t_list **h, t_list **t, char *p,
		int i);
void						print_name(t_file *file, int size);
void						print_full_info(t_list *files);
void						simple_print_col(t_list *head);
void						simple_print(t_list *files);
int							print_path(char *path);
void						print_flags(void);
void						print_list(t_list *files);
t_file						create_file(char *name, char *path);
#endif
