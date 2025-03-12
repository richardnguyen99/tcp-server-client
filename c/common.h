#ifndef __TCP_COMMON_H__
#define __TCP_COMMON_H__ 1

// See
// https://www.gnu.org/software/libc/manual/html_node/Feature-Test-Macros.html
#define _GNU_SOURCE 1

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <inttypes.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// POSIX headers
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// Network headers
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

// Constants

#define BUFSIZ 8192 // Max buffer size
#define PORT "8080" // Port number
#define BACKLOG 10  // Number of pending connections

/**
 * @brief Read a TCP stream from a loop until a terminating newline character
 * is found, or the buffer is full.
 *
 * @param fd Client file descriptor
 * @param buf Allocated buffer
 * @param count Max buffer size
 * @return size_t
 */
size_t readfromstream(int fd, char *buf, size_t count)
{
    size_t total = 0;
    ssize_t n;

    while (total < count)
    {
        n = recv(fd, buf + total, count - total, 0);
        if (n == -1)
        {
            perror("read");
            break;
        }
        if (n == 0)
        {
            break;
        }

        total += (size_t)n;

        if (buf[total - 1] == '\n')
        {
            break;
        }
    }

    return total;
}

size_t writetostream(int fd, const char *buf, size_t count)
{
    size_t total = 0;
    ssize_t n;

    while (total < count)
    {
        n = send(fd, buf + total, count - total, 0);
        if (n == -1)
        {
            perror("write");
            break;
        }

        total += (size_t)n;
    }

    return total;
}

#endif // __TCP_COMMON_H__
