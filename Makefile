TCP_BLOCKING = TCP/blocking
TCP_NON_BLOCKING = TCP/non-blocking
UDP_BLOCKING = UDP/blocking
UDP_NON_BLOCKING = UDP/non-blocking
GCC = gcc
FLAGS = -g -o

all:  tcp udp

tcp: tcp_blocking tcp_non_blocking

tcp_blocking: tcp_blocking_server tcp_blocking_client

tcp_blocking_server: $(TCP_BLOCKING)/server-tcp.o

$(TCP_BLOCKING)/server-tcp.o: $(TCP_BLOCKING)/server-tcp.c
	$(GCC) $(FLAGS) $(TCP_BLOCKING)/server-tcp $(TCP_BLOCKING)/server-tcp.c

tcp_blocking_client: $(TCP_BLOCKING)/client-tcp.o

$(TCP_BLOCKING)/client-tcp.o: $(TCP_BLOCKING)/client-tcp.c
	$(GCC) $(FLAGS) $(TCP_BLOCKING)/client-tcp $(TCP_BLOCKING)/client-tcp.c

tcp_non_blocking: tcp_non_blocking_server tcp_non_blocking_client

tcp_non_blocking_server: $(TCP_NON_BLOCKING)/server-tcp.o

$(TCP_NON_BLOCKING)/server-tcp.o: $(TCP_NON_BLOCKING)/server-tcp.c
	$(GCC) $(FLAGS) $(TCP_NON_BLOCKING)/server-tcp $(TCP_NON_BLOCKING)/server-tcp.c

tcp_non_blocking_client: $(TCP_NON_BLOCKING)/client-tcp.o

$(TCP_NON_BLOCKING)/client-tcp.o: $(TCP_NON_BLOCKING)/client-tcp.c
	$(GCC) $(FLAGS) $(TCP_NON_BLOCKING)/client-tcp $(TCP_NON_BLOCKING)/client-tcp.c

udp: udp_blocking udp_non_blocking

udp_blocking: udp_blocking_server udp_blocking_client

udp_blocking_server: $(UDP_BLOCKING)/server-udp.o

$(UDP_BLOCKING)/server-udp.o: $(UDP_BLOCKING)/server-udp.c
	$(GCC) $(FLAGS) $(UDP_BLOCKING)/server-udp $(UDP_BLOCKING)/server-udp.c

udp_blocking_client: $(UDP_BLOCKING)/client-udp.o

$(UDP_BLOCKING)/client-udp.o: $(UDP_BLOCKING)/client-udp.c
	$(GCC) $(FLAGS) $(UDP_BLOCKING)/client-udp $(UDP_BLOCKING)/client-udp.c

udp_non_blocking: udp_non_blocking_server udp_non_blocking_client

udp_non_blocking_server: $(UDP_NON_BLOCKING)/server-udp.o

$(UDP_NON_BLOCKING)/server-udp.o: $(UDP_NON_BLOCKING)/server-udp.c
	$(GCC) $(FLAGS) $(UDP_NON_BLOCKING)/server-udp $(UDP_NON_BLOCKING)/server-udp.c

udp_non_blocking_client: $(UDP_NON_BLOCKING)/client-udp.o

$(UDP_NON_BLOCKING)/client-udp.o: $(UDP_NON_BLOCKING)/client-udp.c
	$(GCC) $(FLAGS) $(UDP_NON_BLOCKING)/client-udp $(UDP_NON_BLOCKING)/client-udp.c 

clean:
	rm -f $(TCP_BLOCKING)/server-tcp
	rm -f $(TCP_BLOCKING)/client-tcp
	rm -f $(TCP_NON_BLOCKING)/server-tcp
	rm -f $(TCP_NON_BLOCKING)/client-tcp
	rm -f $(UDP_BLOCKING)/server-udp
	rm -f $(UDP_BLOCKING)/client-udp
	rm -f $(UDP_NON_BLOCKING)/server-udp
	rm -f $(UDP_NON_BLOCKING)/client-udp
