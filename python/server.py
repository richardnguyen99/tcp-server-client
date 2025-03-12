# TCP server

import socket
import sys


def readfromstream(conn: socket.socket, size: int) -> bytes:
    data = b""
    total = 0
    while total < size:
        chunk = conn.recv(size)

        if not chunk:
            break

        if len(chunk) == 0:
            break

        total += len(chunk)
        data += chunk

        if chunk.endswith(b"\n"):
            break

    return data


def main():
    msg = "Hello from Server!\n"

    # Get local machine name
    port = 8080
    hosts = socket.getaddrinfo(
        host=None,
        port=port,
        family=socket.AF_UNSPEC,
        type=socket.SOCK_STREAM,
        proto=0,
        flags=socket.AI_PASSIVE,
    )

    # Iterate over the list of hosts
    for family, socktype, proto, canonname, sockaddr in hosts:
        try:
            s = socket.socket(family, socktype, proto)

            # setsockopt(level, optname, value)
            s.setsockopt(
                socket.SOL_SOCKET, socket.SO_REUSEADDR | socket.SO_REUSEPORT, 1
            )

            s.bind(sockaddr)

            print(s)

            break

        except Exception as e:
            print(e, file=sys.stderr)
            continue

    s.listen(socket.SOMAXCONN)

    print("Server is listening on port", port)

    while True:
        conn, addr = s.accept()

        with conn:
            print("Received connection from:", addr)

            # Read data from the client
            data = readfromstream(conn, 8192)

            conn.sendall(msg.encode())

            print(
                "[{}:{}]: {} (read = {} bytes, sent = {} bytes)".format(
                    addr[0], addr[1], data.strip(), len(data), len(msg)
                )
            )


if __name__ == "__main__":
    main()
