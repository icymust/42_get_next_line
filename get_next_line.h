#ifndef GET_NEXT_LINE_H
#define GET_NEXT_LINE_H

#include <stdlib.h>
#include <unistd.h>

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 100
#endif
char buffer[BUFFER_SIZE];

char *get_next_line(int fd);
size_t gnl_strlen(const char *s);
char *gnl_strchr(const char *s, size_t c);
char *gnl_strjoin(char *s1, const char *s2);
char *gnl_strdup(const char *s);
char *gnl_substr(const char *s, size_t start, size_t len);

#endif