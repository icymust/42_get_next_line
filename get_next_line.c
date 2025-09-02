/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmustone <mmustone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 11:53:25 by mmustone          #+#    #+#             */
/*   Updated: 2025/09/02 16:08:39 by mmustone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static int	read_to_stash(int fd, char **stash)
{
	char	buf[BUFFER_SIZE + 1];
	ssize_t	bytes;
	char	*temp;

	while (!gnl_strchr(*stash, '\n') && (bytes = read(fd, buf,
				BUFFER_SIZE)) > 0)
	{
		buf[bytes] = '\0';
		temp = gnl_strjoin(*stash, buf);
		if (!temp)
			return (free(*stash), *stash = NULL, 0);
		*stash = temp;
	}
	if (bytes < 0)
	{
		free(*stash);
		*stash = NULL;
		return (0);
	}
	return (bytes >= 0);
}

static char	*extract_line(char **stash)
{
	char	*nl;
	size_t	len;
	char	*line;
	char	*new_stash;

	nl = gnl_strchr(*stash, '\n');
	len = nl ? (nl - *stash + 1) : gnl_strlen(*stash);
	line = gnl_substr(*stash, 0, len);
	if (!line)
		return (free(*stash), *stash = NULL, NULL);
	new_stash = nl ? gnl_strdup(nl + 1) : NULL;
	free(*stash);
	*stash = new_stash;
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*stash = NULL;

	if (fd < 0 || BUFFER_SIZE <= 0 || !read_to_stash(fd, &stash))
		return (NULL);
	if (!stash || stash[0] == '\0')
		return (free(stash), stash = NULL, NULL);
	return (extract_line(&stash));
}
