# Generic Unix Socket Commands

## List Unix Sockets

List all Unix domain sockets:

```bash
ss -x
```

## Connect to a Stream Socket

Connect to a Unix stream socket using socat:

```bash
socat - UNIX-CONNECT:/path/to/socket
```

## Send Data to a Datagram Socket

Send data to a Unix datagram socket:

```bash
echo "data" | socat -u - UNIX-SENDTO:/path/to/socket
```

## Create a Listening Socket (for testing)

Create a listening Unix socket with socat:

```bash
socat UNIX-LISTEN:/tmp/test.sock -
```

## Check Socket File

Check if a socket file exists:

```bash
ls -la /tmp/*.sock
```