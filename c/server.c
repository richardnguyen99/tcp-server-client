#include "common.h"

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

size_t writetoclient(int fd, const char *buf, size_t count)
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

int main()
{
    int sfd = -1;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    struct addrinfo hints, *res, *p;

    const char *msgfromserver = "Hello from server!\n";

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;     // Allow IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    hints.ai_flags = AI_PASSIVE;     // For wildcard IP address
    hints.ai_protocol = 0;           // Any protocol

    // Get address information
    if (getaddrinfo(NULL, PORT, &hints, &res) != 0)
    {
        perror("getaddrinfo");
        exit(EXIT_FAILURE);
    }

    // Loop through all the results and bind to the first we can
    for (p = res; p != NULL; p = p->ai_next)
    {
        // Create socket
        sfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sfd == -1)
        {
            perror("socket");
            continue;
        }

        int opt = 1;
        if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
        {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }

        // Bind socket to address and port
        if (bind(sfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(sfd);
            perror("bind");
            continue;
        }

        break;
    }

    // Listen for connections
    if (listen(sfd, 1) == -1)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %s...\n", PORT);

    // Keep the server running indefinitely
    for (;;)
    {
        // Accept connection
        int cfd, cport;
        char cname[NI_MAXHOST], buf[BUFSIZ];
        size_t totalread, totalsent;

        cfd = accept(sfd, (struct sockaddr *)&client_addr, &client_len);
        if (cfd == -1)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        // Get client name (hostname)
        if (getnameinfo((struct sockaddr *)&client_addr, client_len, cname, sizeof(cname), NULL, 0, 0) != 0)
        {
            strcpy(cname, "Unknown");
        }

        // Get client port
        cport = ntohs(client_addr.sin_port);

        // Print client information
        printf("Client connected from: %s:%d\n", cname, cport);

        // Read from client
        totalread = readfromstream(cfd, buf, BUFSIZ);

        // Write to client
        totalsent = writetoclient(cfd, msgfromserver, strlen(msgfromserver));

        // Print received data
        // Note: totalread - 2 to remove the newline character
        fprintf(stdout, "[%s:%d]: %.*s (read = %zu bytes, sent = %zu bytes)\n", cname, cport, (int)totalread - 2, buf, totalread, totalsent);

        // Close sockets
        if (close(cfd) == -1)
        {
            perror("close");
            exit(EXIT_FAILURE);
        }
    }

    close(sfd);

    return 0;
}
