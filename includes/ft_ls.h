/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: midrissi <midrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 15:31:17 by midrissi          #+#    #+#             */
/*   Updated: 2019/03/06 23:34:30 by aben-azz         ###   ########.fr       */
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
# define LSFLAGS            "lRaftugb"
# define NZ                 >0
# define F_LIST             (1 << 0)
# define F_RECURSIVE        (1 << 1)
# define F_DOT              (1 << 2)
# define F_REVERSE          (1 << 3)
# define F_SORT_TIME        (1 << 4)
# define F_LAST_ACCESS_TIME (1 << 5)
# define F_SORT_OFF         (1 << 6)
# define F_COLOR            (1 << 7)

t_list	*g_files;
extern char	g_flags;

typedef struct stat		t_stat;
typedef struct dirent	t_dirent;
typedef struct passwd	t_passwd;
typedef struct group	t_group;

typedef struct			s_file
{
	t_stat				stats;
	char				perms[11];
	char				*name;
	char				full_path[PATH_MAX];
}						t_file;

#endif
