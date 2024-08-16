# CS: 744 Multithreaded-Web-Server

## Overview

This project implements an echo client-server application with multi-threading. It includes both single-threaded and multi-threaded server implementations. The purpose is to demonstrate basic socket programming and multi-threading in C/C++.

## Project Structure

- `http_server.cpp`: Implementation of the multi-threaded HTTP server.
- `http_server.hh`: Header file for the multi-threaded HTTP server.
- `simple-client.c`: A simple echo client program.
- `simple-server.c`: A single-threaded echo server program.
- `Makefile`: Used to build the project.
- `README.md`: This file.

## Getting Started

### Prerequisites

- **GCC**: For compiling C programs.
- **pthread Library**: For multi-threading support in C/C++.

### Building the Project

Use the provided `Makefile` to build the project. Run the following command in the terminal:

```sh
make
```

This will compile `http_server.cpp`, `simple-client.c`, and `simple-server.c`, producing the executable binaries.

### Running the Programs

#### Single-Threaded Echo Client and Server

1. **Compile the Programs**

   ```sh
   gcc simple-server.c -o simple-server
   gcc simple-client.c -o simple-client
   ```

2. **Run the Server**

   Open a terminal and start the server:

   ```sh
   ./simple-server 5000
   ```

   The server will start listening on port `5000`.

3. **Run the Client**

   Open another terminal and start the client:

   ```sh
   ./simple-client localhost 5000
   ```

   The client will connect to the server and prompt for a message to send. The server will echo back the message.

#### Multi-Threaded Server

1. **Compile the Multi-Threaded Server**

   ```sh
   g++ http_server.cpp -o http_server -lpthread
   ```

2. **Run the Multi-Threaded Server**

   Open a terminal and start the multi-threaded server:

   ```sh
   ./http_server 5001
   ```

   The server will start listening on port `5001`.

3. **Test Multi-Threading**

   Open multiple terminals and run instances of the client to test the multi-threading functionality of the server.

