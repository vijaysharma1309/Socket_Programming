Transmission Control Protocol (TCP)
===================================

This repository contains simple examples, written in C, which demonstrates usage of 
TCP for reliable / connection oriented communication between two processes running 
on single device or running on different devices.

# Table of Contents
1. TCP Introduction
2. Blocking TCP Server and Blocking TCP Client
3. Non-blocking TCP Server and Non-blocking TCP Client
4. Compilation Steps

## 1. TCP Introduction

TCP is a core member of Internet Protocol Suite (IPS). Before sending messages, 
connection is established between server and client in order to provide guaranteed
delivery of messages.

TCP is a connection-oriented, end-to-end reliable Transport Layer protocol 
designed to applications. The TCP provides for reliable inter-process fit
into a layered hierarchy of protocols which support multi-network communication
between pairs of processes in host computers attached to distinct but 
interconnected computer communication networks.


## 2. Blocking TCP Server and Blocking TCP Client

Sockets can be in either blocking or non-blocking mode. By default, sockets 
are in blocking mode. In blocking mode, the send(), connect(), recv() and accept() 
socket API calls will block indefinitely until the requested action has been performed.

For example, when you call recv() to read from a stream, control isn't returned to 
your program until at least one byte of data is read from the remote site. 
This process of waiting for data to appear is referred to as "blocking". 

## 3. Non-blocking TCP Server and Non-blocking TCP Client

In non-blocking mode, blocking socket APIs do not block instead return immediately if
there is nothing pending.
We can flag these sockets as non-blocking by using fcntl() system call.
```
fcntl(sockfd, F_SETFL, O_NONBLOCK);
```

## 4. Compilation Steps

- To compile TCP examples
```
make tcp
```

- To compile TCP Blocking examples
```
make tcp_blocking
```

- To compile TCP Non-blocking examples
```
make tcp_non_blocking
```

- To remove executable
```
make clean
```
