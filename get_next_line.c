/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmustone <mmustone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 11:53:25 by mmustone          #+#    #+#             */
/*   Updated: 2025/09/03 12:34:00 by mmustone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** Идея решения:
** - Между вызовами храним накопленный буфер "stash" (static).
** - Дочитываем из fd порциями BUFFER_SIZE и дописываем в stash,
**   пока не появится '\n' или не наступит EOF (read_to_stash).
** - Возвращаем одну строку из начала stash (до '\n' включительно
**   либо до конца), а остаток оставляем в stash (extract_line).
**
** Почему это быстро и не течёт:
** - append_buf растит stash с удвоением ёмкости (64, 128, 256, ...):
**   амортизированно O(n) на общий объём данных, без квадратичной деградации.
** - Строго освобождаем память: если остатка после извлечения строки нет —
**   обнуляем stash (без аллокации пустых строк).
**
** Ограничение:
** - Версия для одного дескриптора (mandatory): используем static-переменные
**   без отображения состояния на fd. Для бонуса нужен словарь fd->stash.
*/

#include "get_next_line.h"

/*
** Локальный memcpy-эквивалент.
** - Не обрабатывает перекрытие (как стандартный memcpy).
** - Используется для копирования блоков без подключения <string.h>.
*/
static void	*loc_memcpy(void *dst, const void *src, size_t n)
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
** append_buf
** Назначение:
**   - Дописать к строке stash блок buf длиной add.
**   - При необходимости увеличить ёмкость stash экспоненциально (x2),
**     скопировав старые данные в новый буфер.
**
** Контракты:
**   - На выходе: stash остаётся нуль-терминированной строкой.
**   - Инварианты: *cap >= *len + 1 (под '\0').
**   - Возврат: 1 — успех, 0 — ошибка malloc (stash невалиден, выше чистим).
**
** Почему стартуем с 64:
**   - Небольшое базовое значение уменьшает число realLOC при маленьких файлах,
**     но быстро растёт для больших (двойками).
*/
static int	append_buf(char **stash, size_t *len, size_t *cap,
                        const char *buf, size_t add)
{
    size_t	new_len;
    size_t	new_cap;
    char	*ns;

    if (add == 0)
        return (1);
    new_len = *len + add;
    if (new_len + 1 > *cap)
    {
        new_cap = (*cap ? *cap : 64);
        while (new_cap < new_len + 1)
            new_cap *= 2;
        ns = (char *)malloc(new_cap);
        if (!ns)
            return (0);
        if (*len && *stash)
            loc_memcpy(ns, *stash, *len);  /* переносим старое содержимое */
        free(*stash);
        *stash = ns;
        *cap = new_cap;
    }
    /* дозаписываем свежепрочитанные байты и ставим '\0' */
    loc_memcpy(*stash + *len, buf, add);
    (*stash)[new_len] = '\0';
    *len = new_len;
    return (1);
}

/*
** read_to_stash
** Назначение:
**   - Читать порциями BUFFER_SIZE и дописывать в stash, пока:
**       • не встретим '\n' в свежепрочитанном блоке (достаточно для одной строки),
**       • или не наступит EOF,
**       • или не случится ошибка.
**
** Особенности:
**   - Сначала проверяем, нет ли уже '\n' в существующем stash — тогда читать не надо.
**   - Ищем '\n' в buf, а не в целом stash — это дешевле, чем каждый раз сканировать всё.
**   - При ошибке read/alloc: освобождаем stash и сбрасываем длину/ёмкость.
*/
static int  read_to_stash(int fd, char **stash, size_t *len, size_t *cap)
{
    char    buf[BUFFER_SIZE + 1];
    ssize_t bytes;
    int     found_nl;

    if (*stash && gnl_strchr(*stash, '\n'))
        return (1);
    found_nl = 0;
    while ((bytes = read(fd, buf, BUFFER_SIZE)) > 0)
    {
        buf[bytes] = '\0';
        if (gnl_strchr(buf, '\n'))
            found_nl = 1;  /* можно остановиться после аппенда */
        if (!append_buf(stash, len, cap, buf, (size_t)bytes))
            return (free(*stash), *stash = NULL, *len = 0, *cap = 0, 0);
        if (found_nl)
            break;
    }
    if (bytes < 0)
        return (free(*stash), *stash = NULL, *len = 0, *cap = 0, 0);
    return (1);
}

/*
** extract_line
** Назначение:
**   - Выделить одну логическую строку из начала stash:
**       • если есть '\n' — берём до него включительно,
**       • если нет — возвращаем всё содержимое (последняя строка без '\n').
**   - Остаток (после строки) переносится в новый буфер; если остатка нет —
**     stash = NULL (никаких «пустых» аллокаций).
**
** Возврат:
**   - Указатель на новую строку (heap) или NULL при ошибке аллокации.
**   - В случае ошибки аллокации весь stash очищается (чтобы не течь).
*/
static char	*extract_line(char **stash, size_t *len, size_t *cap)
{
    char	*nl;
    size_t	line_len;
    char	*line;
    char	*new_stash;
    size_t	rem;

    nl = gnl_strchr(*stash, '\n');
    if (nl)
        line_len = (size_t)(nl - *stash) + 1;
    else
        line_len = *len;

    /* копируем целевую строку для возврата */
    line = gnl_substr(*stash, 0, line_len);
    if (!line)
    {
        free(*stash);
        *stash = NULL;
        *len = 0;
        *cap = 0;
        return NULL;
    }

    /* считаем остаток и переносим его в новый stash (если он есть) */
    rem = (*len > line_len) ? (*len - line_len) : 0;
    if (rem > 0 && nl && nl[1] != '\0')
        new_stash = gnl_substr(*stash, line_len, rem);
    else
        new_stash = NULL;

    /* старый stash больше не нужен */
    free(*stash);
    *stash = new_stash;
    *len = rem;
    *cap = rem; /* при следующем чтении ёмкость при необходимости вырастет */
    return line;
}

/*
** get_next_line
** Поток выполнения:
**   1) Валидация параметров и дозаполнение stash (read_to_stash).
**   2) Если данных нет — чистим и возвращаем NULL.
**   3) Иначе извлекаем одну строку (extract_line) и возвращаем её.
**
** Замечания:
**   - static-переменные => состояние сохраняется между вызовами.
**   - Не потокобезопасно, один fd (mandatory).
**   - Сложность: O(M) по числу реально прочитанных байт до возвращаемой строки.
*/
char	*get_next_line(int fd)
{
    static char	*stash;      /* накопленный буфер между вызовами */
    static size_t stash_len;  /* текущая длина stash (без '\0') */
    static size_t stash_cap;  /* текущая ёмкость stash (включая место под '\0') */

    if (fd < 0 || BUFFER_SIZE <= 0 || !read_to_stash(fd, &stash, &stash_len, &stash_cap))
        return (NULL);
    if (!stash || stash[0] == '\0')
        return (free(stash), stash = NULL, stash_len = 0, stash_cap = 0, NULL);
    return (extract_line(&stash, &stash_len, &stash_cap));
}
