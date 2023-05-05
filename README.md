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

Before running the server, you can custom your owe controller in `main.c`:

```c
// main.c
void custom_controller(request_t *req, response_t *res) {
    GET(req, "/") {
        printf("This is a %s request from %s\n",
               req->header->method, req->header->route);
        generate_response(res, 200, "OK", "<h1> Hello World! </h1>");
    }

    POST(req, "/hello") {
        printf("This is a %s request from %s\n",
               req->header->method, req->header->route);
        generate_response(res, 200, "OK", "Hello World!");
    }
}
```

and inject to `server_run` function.

```c
// main.c
server_run(sockfd, custom_controller);
```

Then run the server:

```
$ ./httpc localhost 3490
```

Do a simple test with curl:

```
$ curl -X GET http://localhost:3490/
$ curl -X POST http://localhost:3490/hello
```

output:

```
[server] listening on localhost:3490
[server] waiting for connection...

[server] got connection from ::1
This is a GET request from /

[server] got connection from ::1
This is a POST request from /hello
```

Open web browser and visit `http://localhost:3490/` can see the response.


## License

MIT
