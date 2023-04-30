# http-c

Implementation of a simple HTTP server in C.

Inspired by [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/).

## Features

Simple http server that can serve static files. Support basic HTTP methods (GET, POST, DELETE). Multi-threaded server that can handle multiple requests at the same time. Basic controller-like macro to handle url requests.

## Build

```
$ make
```

## Usage

If no hostname and port are provided, the server will listen on localhost:3490.

```
$ ./httpc <hostname> <port>
```

## Example

Do a simple test with curl:

```
$ curl -X GET http://localhost:3490/
```

output:

```
server: got connection from ::1
METHOD: GET
URL: /
PROT: HTTP/1.1
```

## License

MIT
