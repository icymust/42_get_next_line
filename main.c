#include "get_next_line.h"
#include "get_next_line_bonus.h"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main(void) {

    //mandatory
    // int fd = open("test.txt", O_RDONLY);
    // if (fd < 0) return 1;
    // char *line;
    // while ((line = get_next_line(fd))) {
    //     printf("%s", line);
    //     free(line);
    // }
    // close(fd);
    // return 0;

    //bonus
    // int   a = open("a.txt", O_RDONLY);
    // int   b = open("b.txt", O_RDONLY);
    // char *l1, *l2;

    // if (a < 0 || b < 0) return 1;
    // while ((l1 = get_next_line(a)) || (l2 = get_next_line(b)))
    // {
    //     if (l1) { printf("A:%s\n", l1); free(l1); }
    //     if (l2) { printf("B:%s", l2); free(l2); }
    // }
    // close(a);
    // close(b);
    // return 0;
}