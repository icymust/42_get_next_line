#ifndef GET_NEXT_LINE_H
#define GET_NEXT_LINE_H

#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 100
char buffer[BUFFER_SIZE];

char *get_next_line(int fd);

#endif