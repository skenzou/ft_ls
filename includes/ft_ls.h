/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: midrissi <midrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 15:31:17 by midrissi          #+#    #+#             */
/*   Updated: 2019/04/01 20:19:30 by midrissi         ###   ########.fr       */
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
# define LSFLAGS			"lRarftug"
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
# define ISEXEC(m)			(((m) & S_IXUSR) == S_IXUSR)
# define MAX_PATH_LEN		4096

t_list						*g_files;
extern char					g_flags;

typedef struct stat			t_stat;
typedef struct dirent		t_dirent;
typedef struct passwd		t_passwd;
typedef struct group		t_group;

typedef struct				s_file
{
	t_stat					stats;
	size_t						size;
	char					perms[11];
	char					*name;
	char					full_path[MAX_PATH_LEN];
}							t_file;

#endif
