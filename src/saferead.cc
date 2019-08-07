#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

#include "saferead.h"

/**
* read len bytes or die trying
*****************************************************************/
ssize_t saferead(int fd, char *buf, ssize_t want)
{
	ssize_t got = 0;
	while (got < want)
	{
		ssize_t len = read(fd, &buf[got], want);
		if (len == -1)
			return -1;
		if (len == 0)
			break;

		got += len;
		want -= len;
	}
	return got;
}
