/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minilib.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/08 19:56:31 by vneelix           #+#    #+#             */
/*   Updated: 2021/08/09 20:16:08 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINILIB_H
# define MINILIB_H

# include <math.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/stat.h>
# include <sys/fcntl.h>

# include <string.h>

double		ft_atof(const char *str);
int			ft_atoi(const char *ptr);
long		ft_atol(const char *ptr);
long long	ft_atoll(const char *ptr);
void		ft_bzero(void *ptr, size_t n);
void		*ft_memset(void *ptr, int c, size_t n);
char		*ft_read_file_fstat(const char *path);
char		**ft_strsplit(const char *str, const char sep);
int			is_number(const char *str);
void		*ft_memcpy(void *dest, const void *src, size_t n);
int			error_callback(int err_code,
				const char *target, const char *reason, char **err);

#endif
