Socket_Programming
==================
Server Client design using Socket Programming in C (For FUN and LEARNING). 

# Socket Programming Applications / Examples

This repository contains applications, written in C, which demonstrates underlying
concepts of Socket Programming for communication between two processes running on 
single device or running on different devices.

Each directory represent a unique topic (TCP, UDP) and contains a Makefile and a simple
example to understand the concept.

## TCP

This directory contains example TCP server and TCP client that demonstrates
TCP communication between server and client in blocking and non-blocking mode.

Kindly refer the [TCP/README.md](TCP/README.md) for further details and usage.

## UDP

This directory contains example UDP server and UDP client that demonstrates
UDP communication between server and client in blocking and non-blocking mode.

Kindly refer the [UDP/README.md](UDP/README.md) for further details and usage.

## Compilation Steps

- To compile whole project
```
make all
```

- To compile TCP examples
```
make tcp
```

- To compile UDP examples
```
make udp
```

- To remove executables
```
make clean
```

Please refer [Socket_Programming/socket_details.txt](Socket_Programming/socket_details.txt)
for general information about socket programming and wireshark captures.
