import socket


def main():
    port = 8080
    hosts = socket.getaddrinfo(
        host="localhost",
        port=port,
        family=socket.AF_UNSPEC,
        type=socket.SOCK_STREAM,
        proto=0,
    )

    for family, socktype, proto, canonname, sockaddr in hosts:
        try:
            s = socket.socket(family, socktype, proto)
            s.connect(sockaddr)

            print(s)

            break

        except Exception as e:
            print(e)
            continue

    if not s:
        print("Failed to connect")
        return

    msg = "Hello from Client!\n"
    s.sendall(msg.encode())

    data = s.recv(1024)
    print(
        "Received {} (read = {} bytes, sent = {} bytes)".format(
            data.decode().strip(), len(data), len(msg)
        )
    )


if __name__ == "__main__":
    main()
