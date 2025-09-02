/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmustone <mmustone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 13:05:16 by mmustone          #+#    #+#             */
/*   Updated: 2025/09/02 14:50:45 by mmustone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	gnl_strlen(const char *s)
{
	size_t	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
		i++;
	return (i);
}

char	*gnl_strchr(const char *s, size_t c)
{
	if (!s)
		return (NULL);
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if ((char)c == '\0')
		return ((char *)s);
	return (NULL);
}

char	*gnl_strjoin(char *s1, const char *s2)
{
	size_t	len1;
	size_t	len2;
	char	*ns;
	size_t	i;
	size_t	j;

	len1 = gnl_strlen(s1);
	len2 = gnl_strlen(s2);
	ns = malloc(len1 + len2 + 1);
	if (!ns)
		return (NULL);
	i = 0;
	if (s1)
	{
		while (s1[i])
		{
			ns[i] = s1[i];
			i++;
		}
	}
	j = 0;
	while (s2 && s2[j])
	{
		ns[i + j] = s2[j];
		j++;
	}
	ns[i + j] = '\0';
	free(s1);
	return (ns);
}

//проверить функции и интегрировать в код

char	*gnl_substr(const char *s, size_t start, size_t len)
{
	size_t	slen;
	char	*sub;
	size_t	i;

	if (!s)
		return (NULL);
	slen = gnl_strlen(s);
	if (start >= slen)
		return (gnl_strjoin(NULL, "")); // пустая строка
	if (len > slen - start)
		len = slen - start;
	sub = malloc(len + 1);
	if (!sub)
		return (NULL);
	i = 0;
	while (i < len && s[start + i])
	{
		sub[i] = s[start + i];
		i++;
	}
	sub[i] = '\0';
	return (sub);
}

char	*gnl_strdup(const char *s)
{
	if (!s)
		return (NULL);
	size_t len = gnl_strlen(s);
	char *dup = malloc(len + 1);
	if (!dup)
		return (NULL);
	for (size_t i = 0; i < len; i++)
		dup[i] = s[i];
	dup[len] = '\0';
	return (dup);
}
