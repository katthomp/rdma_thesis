CC=gcc
FLAGS=-Wall -Wextra -Werror -g
LIBS=-libverbs -lrdmacm

SRCS=mini_rdma_client.c mini_rdma_server.c mini_rdma_cmn.c
OBJS=$(SRCS:.c=.o)
PROG=mini_rdma


all: $(PROG)


.c.o:
	$(CC) $(FLAGS) -o $@ $(OBJS) $(LIBS)

$(PROG): $(OBJS)
	$(CC) $(FLAGS) -o $(@) $(OBJS) $(LIBS)

clean:
	$(RM) *.o *~$(PROG)
