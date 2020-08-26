User Datagram Protocol (UDP)
============================

This repository contains simple examples, written in C, which
demonstrates usage of UDP for unreliable / connection less
communication between two processes running on single device 
or running on different devices.

# Table of Contents
1. UDP Introduction
2. Blocking UDP Server and Blocking UDP Client
3. Non-blocking UDP Server and Non-blocking UDP Client
4. Compilation Steps

## 1. UDP Introduction

UDP is a core member of Internet Protocol Suite (IPS). Messages sent 
using UDP are called datagrams. A client can send messages to other hosts 
on an Internet Protocol (IP) network without any prior communications to 
set up special transmissions channels or data paths.

UDP uses a minimal amount of protocol mechanisms in that it requires no handshake. 
Rather UDP uses a checksum for data integrity and port numbers for addressing 
different functions at the source and destination of the datagram.

> Advantages of using UDP:
	- It is ideal for time sensitive applications.
	- Absence of retransmission of messages makes it suitable for real-time
	  applications such as live streaming or voice over IP (VOIP).

> Disadvantages of usign UDP:
	- It provides no guarantees to the upper layer protocol for message delivery. 
	- UDP protocol layer retains no backup of UDP messages once sent. 
	Therefore messages are non-recoverable once sent.

## 2. Blocking UDP Server and Blocking UDP Client

Sockets can be in either blocking or non-blocking mode. By default, sockets are
in blocking mode. In blocking mode, the send(), recv() etc. socket API calls will 
block indefinitely until the requested action has been performed.

For example, when you call recv() to read from a stream, control isn't returned to 
your program until at least one byte of data is read from the remote site. 
This process of waiting for data to appear is referred to as "blocking". 

## 3. Non-blocking UDP Server and Non-blocking UDP Client

In non-blocking mode, blocking socket APIs do not block instead return immediately if
there is nothing pending.
We can flag these sockets as non-blocking by using fcntl() system call.

```
fcntl(sockfd, F_SETFL, O_NONBLOCK);
```

## 4. Compilation Steps

- To compile UDP examples
```
make udp
```

- To compile UDP Blocking examples
```
make udp_blocking
```

- To compile UDP Non-blocking examples
```
make udp_non_blocking
```

- To remove executable
```
make clean
```
