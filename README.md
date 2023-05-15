# http-c

Implementation of a simple HTTP server in C.

Inspired by [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/).

## Features

Simple http server that can serve static files. Support basic HTTP methods (GET, POST, DELETE). Basic controller-like macro to handle url requests. The server use a lite thread pool to handle multiple requests.

## Build

```
$ make
```

## Usage

If no hostname and port are provided, the server will listen on localhost:3490.

```
$ ./httpc [-hostname <hostname>] [-port <port>] [-thread <thread_number>]
```

## Example

Before running the server, you can custom your owe controller in `main.c`:

```c
// main.c
void custom_controller(request *req, response *res) {
    GET(req, "/") {
        printf("This is a %s request from %s\n",
               req->header->method, req->header->route);
        generate_response(res, 200, "OK", "<h1> Hello World! </h1>");
        return;
    }

    POST(req, "/hello") {
        printf("This is a %s request from %s\n",
               req->header->method, req->header->route);
        generate_response(res, 200, "OK", "Hello World!");
        return;
    }

    generate_response(res, 404, "Not Found", "");
}
```

and inject to `server_run` function.

```c
// main.c
server_run(sockfd, custom_controller);
```

Then run the server:

```
$ ./httpc -hostname localhost -port 3490 -thread 4
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
[threadpool] thread: thpool-0 working on task
This is a GET request to route /

[server] got connection from ::1
[threadpool] thread: thpool-2 working on task
This is a POST request to route /hello
```

Open web browser and visit `http://localhost:3490/` can see the response.

![Screenshot 2023-05-05 at 1 36 33 PM](https://user-images.githubusercontent.com/24312717/236564197-76c74a3e-6ae4-460c-b358-d2e204b1171e.png)


## TEST

I insert a usleep(50000) in the controller to simulate a long time request. Then

you can use [`drill`](https://github.com/fcsonline/drill) to  test the server.

```
$ ./httpc -hostname localhost -port 3490 -thread 4
```

run the test:

```
$ drill --benchmark benchmark.yml --stats
```

1000 request to a server with  4 thread:

```
Time taken for tests      13.4 seconds
Total requests            1000
Successful requests       1000
Failed requests           0
Requests per second       74.51 [#/sec]
Median time per request   268ms
Average time per request  265ms
Sample standard deviation 19ms
99.0'th percentile        274ms
99.5'th percentile        276ms
99.9'th percentile        276ms
```

1000 request to a server with 32 threads:

```
Time taken for tests      2.8 seconds
Total requests            1000
Successful requests       1000
Failed requests           0
Requests per second       352.38 [#/sec]
Median time per request   55ms
Average time per request  55ms
Sample standard deviation 2ms
99.0'th percentile        58ms
99.5'th percentile        59ms
99.9'th percentile        59ms
```

## License

MIT
