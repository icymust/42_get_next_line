/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmustone <mmustone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 16:25:55 by mmustone          #+#    #+#             */
/*   Updated: 2025/09/08 17:11:26 by mmustone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 100
# endif

# define MAXF 10000

char	*get_next_line(int fd);
int		gnl_strlen(const char *s);
char	*gnl_strchr(const char *s, size_t c);
char	*gnl_strjoin(char *s1, const char *s2);
char	*gnl_strdup(const char *s);
char	*gnl_substr(const char *s, size_t start, size_t len);

#endif