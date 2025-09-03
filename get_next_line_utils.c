/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmustone <mmustone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 13:05:16 by mmustone          #+#    #+#             */
/*   Updated: 2025/09/03 12:34:00 by mmustone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/*
** gnl_strlen
** Возвращает длину C-строки s. Если s == NULL, возвращает 0.
*/
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

/*
** gnl_strchr
** Ищет первое вхождение символа c (как int, по стандарту C) в строке s.
** Возвращает указатель внутрь s или NULL. Поддерживает поиск '\0'.
*/
char	*gnl_strchr(const char *s, int c)
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

/*
** gnl_memcpy (локальная)
** Копирует n байт из src в dst. Не обрабатывает перекрытие (как memcpy).
** Используется для ускорения операций копирования без <string.h>.
*/
static void	*gnl_memcpy(void *dst, const void *src, size_t n)
{
    size_t				i;
    unsigned char		*d;
    const unsigned char	*s;

    if (!dst && !src)
        return (NULL);
    d = (unsigned char *)dst;
    s = (const unsigned char *)src;
    i = 0;
    while (i < n)
    {
        d[i] = s[i];
        i++;
    }
    return (dst);
}

/*
** gnl_strjoin
** Выделяет новую строку = s1 + s2, возвращает её и ОСВОБОЖДАЕТ s1
** ТОЛЬКО при УСПЕШНОЙ аллокации. При ошибке malloc возвращает NULL
** и НЕ трогает s1 (чтобы избежать double free на стороне вызывающего).
*/
char	*gnl_strjoin(char *s1, const char *s2)
{
    size_t	len1;
    size_t	len2;
    char	*ns;

    len1 = gnl_strlen(s1);
    len2 = gnl_strlen(s2);
    ns = (char *)malloc(len1 + len2 + 1);
    if (!ns)
        return (NULL);                 // Не освобождаем s1 при ошибке
    if (len1)
        gnl_memcpy(ns, s1, len1);
    if (len2)
        gnl_memcpy(ns + len1, s2, len2);
    ns[len1 + len2] = '\0';
    free(s1);                          // Освобождаем s1 только при успехе
    return (ns);
}

/*
** gnl_substr
** Создаёт подстроку из s, начиная с позиции start, длиной len (или меньше
** до конца строки). Если start >= длине s, возвращает пустую строку.
*/
char	*gnl_substr(const char *s, size_t start, size_t len)
{
    size_t	slen;
    char	*sub;
    size_t	i;

    if (!s)
        return (NULL);
    slen = gnl_strlen(s);
    if (start >= slen)
        return (gnl_strdup(""));       // Пустая строка
    if (len > slen - start)
        len = slen - start;
    sub = (char *)malloc(len + 1);
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

/*
** gnl_strdup
** Дублирует строку s (heap). Возвращает NULL при ошибке malloc.
*/
char	*gnl_strdup(const char *s)
{
    size_t	len;
    char	*dup;
    size_t	i;

    if (!s)
        return (NULL);
    len = gnl_strlen(s);
    dup = (char *)malloc(len + 1);
    if (!dup)
        return (NULL);
    i = 0;
    while (i < len)
    {
        dup[i] = s[i];
        i++;
    }
    dup[len] = '\0';
    return (dup);
}
