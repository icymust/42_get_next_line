/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmustone <mmustone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 11:53:25 by mmustone          #+#    #+#             */
/*   Updated: 2025/09/05 18:13:18 by mmustone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static int	handle_error(char **stash)
{
	if (stash && *stash)
	{
		free(*stash);
		*stash = NULL;
	}
	return (0);
}

static int	check_stash(char **stash)
{
	if (!stash)
		return (0);
	if (*stash && gnl_strchr(*stash, '\n'))
		return (1);
	return (0);
}

static int	read_to_stash(int fd, char **stash)
{
    char	*buf;
    ssize_t	bytes;
    char	*tmp;
    int		found_nl;

    buf = malloc(BUFFER_SIZE + 1);
    if (!buf)
        return (handle_error(stash));
    if (check_stash(stash))
    {
        free(buf);
        return (1);
    }
    found_nl = 0;
    bytes = read(fd, buf, BUFFER_SIZE);
    while (bytes > 0)
    {
        buf[bytes] = '\0';
        if (gnl_strchr(buf, '\n'))
            found_nl = 1;
        tmp = gnl_strjoin(*stash, buf);
        if (!tmp)
        {
            free(buf);
            return (handle_error(stash));
        }
        *stash = tmp;
        if (found_nl)
            break ;
        bytes = read(fd, buf, BUFFER_SIZE);
    }
    free(buf);
    if (bytes < 0)
        return (handle_error(stash));
    return (1);
}

static char	*extract_line(char **stash)
{
	char	*nl;
	size_t	len;
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
	static char	*stash;

	if (fd < 0 || BUFFER_SIZE <= 0 || !read_to_stash(fd, &stash))
		return (NULL);
	if (!stash || stash[0] == '\0')
	{
		handle_error(&stash);
		return (NULL);
	}
	return (extract_line(&stash));
}
