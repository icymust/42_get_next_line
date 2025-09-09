/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmustone <mmustone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 16:12:49 by mmustone          #+#    #+#             */
/*   Updated: 2025/09/08 17:08:18 by mmustone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

int	handle_error(char **stash)
{
	if (stash && *stash)
	{
		free(*stash);
		*stash = NULL;
	}
	return (0);
}

int	append_chunk(char **stash, char *buf, ssize_t n)
{
	char	*tmp;

	buf[n] = '\0';
	tmp = gnl_strjoin(*stash, buf);
	if (!tmp)
		return (0);
	*stash = tmp;
	return (1);
}

int	read_to_stash(int fd, char **stash)
{
	char	*buf;
	ssize_t	bytes;

	buf = malloc(BUFFER_SIZE + 1);
	if (!buf)
		return (handle_error(stash));
	bytes = read(fd, buf, BUFFER_SIZE);
	while (bytes > 0)
	{
		if (!append_chunk(stash, buf, bytes))
		{
			free(buf);
			return (handle_error(stash));
		}
		if (gnl_strchr(buf, '\n'))
			break ;
		bytes = read(fd, buf, BUFFER_SIZE);
	}
	free(buf);
	if (bytes < 0)
		return (handle_error(stash));
	return (1);
}

char	*extract_line(char **stash)
{
	char	*nl;
	int		len;
	char	*line;
	char	*new_stash;

	nl = gnl_strchr(*stash, '\n');
	if (nl)
		len = nl - *stash + 1;
	else
		len = gnl_strlen(*stash);
	line = gnl_substr(*stash, 0, len);
	if (!line)
	{
		handle_error(stash);
		return (NULL);
	}
	if (nl)
		new_stash = gnl_strdup(nl + 1);
	else
		new_stash = NULL;
	free(*stash);
	*stash = new_stash;
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*stash[MAXF];

	if (fd < 0 || BUFFER_SIZE <= 0 || fd >= MAXF || !read_to_stash(fd,
			&stash[fd]))
		return (NULL);
	if (!stash[fd] || stash[fd][0] == '\0')
	{
		handle_error(&stash[fd]);
		return (NULL);
	}
	return (extract_line(&stash[fd]));
}
