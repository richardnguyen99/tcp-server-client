#include "common.h"

int main(void)
{
    int cfd = -1, s;
    struct addrinfo hints, *res, *p;
    size_t totalread, totalsent;
    char buf[BUFSIZ];

    const char *msgfromclient = "Hello from client!\n";

    // Get address information

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    hints.ai_protocol = 0;

    s = getaddrinfo("localhost", PORT, &hints, &res);
    if (s != 0)
    {
        perror("getaddrinfo");
        return 1;
    }

    // Create socket
    for (p = res; p != NULL; p = p->ai_next)
    {
        cfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (cfd == -1)
        {
            perror("socket");
            continue;
        }

        // Connect to server
        if (connect(cfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(cfd);
            perror("connect");
            continue;
        }

        break;
    }

    if (p == NULL)
    {
        fprintf(stderr, "Failed to connect\n");
        return 2;
    }

    // Send message to server

    totalsent = writetostream(cfd, msgfromclient, strlen(msgfromclient));

    totalread = readfromstream(cfd, buf, BUFSIZ);
    buf[totalread] = '\0';

    fprintf(stdout, "Received: %s (read = %zu, sent = %zu)\n", buf, totalread, totalsent);

    close(cfd);
    freeaddrinfo(res);

    return 0;
}
