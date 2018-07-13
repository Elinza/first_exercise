SER_EXE = server
SER_OBJS = tcpserver.o
CLI_EXE = client
CLI_OBJS = tcpclient.o

all:$(SER_EXE) $(CLI_EXE)

$(SER_EXE):$(SER_OBJS)
	$(CC) -o $(SER_EXE) $(SER_OBJS)

$(CLI_EXE) : $(CLI_OBJS)
	$(CC) -o $(CLI_EXE) $(CLI_OBJS)
$.o:$.c
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: clean

clean:
	rm -f $(SER_EXE) $(SER_OBJS) $(CLI_EXE) $(CLI_OBJS)
