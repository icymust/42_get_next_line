#ifndef GET_NEXT_LINE_H
#define GET_NEXT_LINE_H

#include <stdlib.h>
#include <unistd.h>

/*
** BUFFER_SIZE — размер временного буфера чтения для read(2).
** Может переопределяться через -D BUFFER_SIZE=... при компиляции.
*/
#ifndef BUFFER_SIZE
#define BUFFER_SIZE 100
#endif

/*
** Публичный интерфейс:
** - get_next_line: читает из fd и возвращает следующую строку
**   (включая '\n', если он есть). Возвращает NULL при ошибке или EOF
**   без данных.
**
** Вспомогательные функции (реализация своих аналогов libft),
** разрешённые правилами проекта:
** - gnl_strlen   : длина строки
** - gnl_strchr   : поиск символа в строке (аргумент c — int, как в libc)
** - gnl_strjoin  : конкатенация, освобождает s1 ТОЛЬКО при успехе
** - gnl_strdup   : дублирование строки
** - gnl_substr   : подстрока [start, start+len)
*/
char *get_next_line(int fd);
size_t gnl_strlen(const char *s);
char *gnl_strchr(const char *s, int c);
char *gnl_strjoin(char *s1, const char *s2);
char *gnl_strdup(const char *s);
char *gnl_substr(const char *s, size_t start, size_t len);

#endif