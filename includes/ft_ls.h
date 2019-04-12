/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: midrissi <midrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 15:31:17 by midrissi          #+#    #+#             */
/*   Updated: 2019/04/13 01:39:14 by midrissi         ###   ########.fr       */
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
# include <sys/xattr.h>
# include <sys/acl.h>
# define LSFLAGS			"lRartG"
# define F_LIST				(1 << 0)
# define F_RECURSIVE		(1 << 1)
# define F_DOT				(1 << 2)
# define F_REVERSE			(1 << 3)
# define F_LAST_MODIF		(1 << 4)
# define F_COLOR			(1 << 5)
# define ANSI_RED			"\x1b[31m"
# define ANSI_BOLDGREEN		"\x1b[1m\x1b[32m"
# define ANSI_GREEN			"\x1b[32m"
# define ANSI_YELLOW		"\x1b[33m"
# define ANSI_BOLDBLUE		"\x1b[1m\x1b[34m"
# define ANSI_BLUE			"\x1b[34m"
# define ANSI_MAGENTA		"\x1b[1m\x1b[35m"
# define ANSI_CYAN			"\x1b[36m"
# define ANSI_BOLDCYAN		"\x1b[1m\x1b[36m"
# define ANSI_RESET			"\x1b[0m"
# define ANSI_PURPLE		"\x1b[35m"
# define ANSI_IGREEN		"\x1b[42m"
# define ANSI_IYELLOW		"\x1b[43m\x1b[34m"
# define ANSI_IBLUE			"\x1b[46m\x1b[34m"
# define MAX_PATH_LEN		PATH_MAX
# define SIX_MONTHS			15778476

extern char					g_flags;

typedef struct stat			t_stat;
typedef struct dirent		t_dirent;
typedef struct passwd		t_passwd;
typedef struct group		t_group;

typedef struct				s_file
{
	t_stat					stats;
	size_t					namesize;
	char					perms[11 + 1];
	char					*name;
	char					*path;
	char					full_path[MAX_PATH_LEN];
	char					printed;
	int						nbfiles;
	time_t					time;
	long					ntime;
	char					*pwd;
	char					*group;
}							t_file;

/*
** SIMPLE_PRINT.C
*/
void						simple_print(t_list *head, int argc);
int							continue_reading(t_list *head);
void						print_name(t_file *file, int size);
/*
** PRINT_FULL_INFO.C
*/
void						print_full_info(t_list *head, int header);
/*
** PRINT_UTILS.C
*/
void						print_time(t_file *file);
int							print_link(t_file *file);
int							print_head(char *path, int header, t_list *files);
int							lsprint(char *string, int namesize, int padding,
																char *color);
int							lsputnbr(int nbr, int padding, int afterspace);
/*
** CREATE_FILE.C
*/
t_file						create_file(char *name, char *path);
/*
** UTILS.C
*/
int							sort_args_time(int argc, t_list *argv[argc],
																	char rev);
int							sort_args(int argc, char **argv, char reverse);
void						ft_listdel(t_list *head);
void						print_err(char *name);
int							compare_folder_time(t_list *first, t_list *second);
/*
** GETERS.C
*/
int							get_max_name_length(t_list *files);
int							get_col(t_list *files, int *nbfile);
long long					get_totalsize(t_list *files);
char						get_extended(t_file *file);
int							get_term_colsize(void);
/*
** HANDLERS.C
*/
void						handle_notdir(char *name, t_list **fiflnks);
void						handle_fiflnks(t_list *fiflnks, t_list *head);
void						set_max_length(t_list *files, int len[4]);
char						third_permission(int m, char type_user);
/*
** LIST.C
*/
void						list_dir(DIR *dir, t_list **head, char *path,
																	long ntime);
int							insert_asc_loop(t_list **head, t_list *needle,
																char reverse);
int							insert_asc_id(t_list *curr, t_list *needle,
																char reverse);
int							insert_asc_r(t_list **head, t_list *needle,
																	int ret);
int							insert_asc(t_list **head, t_list *needle, int ret);
/*
** LIST2.C
*/
int							insert_time(t_list **head, t_list *needle, int ret);
int							insert_time_r(t_list **head, t_list *needle,
																	int ret);
int							insert_time_loop(t_list **head, t_list *needle,
																char reverse);
int							insert_time_id(t_list **head, t_list *needle,
																char reverse);
/*
** FLAGS.C
*/
void						usage(char c);
char						**set_lsflags(int *argc, char **argv);
/*
** MAIN.C
*/
void						ft_ls_r(t_file *file);

#endif
