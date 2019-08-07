#ifndef saferead_H
#define saferead_H

#include <unistd.h>

ssize_t saferead(int fd, char *buf, ssize_t want);

#endif
