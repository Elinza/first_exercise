C_EXE = selectcli
C_OBJS = selectcli.o
CXXFLAGS = -std=gnu+11

S_EXE = selectser
S_OBJS = selectser.o

T_EXE = tcpselect
T_OBJS = tcpselect.o
CXXFLAGS = -std=gnu+11

all:$(C_EXE) $(S_EXE) $(T_EXE)

$(C_EXE):$(C_OBJS)
	$(CXX) -o $(C_EXE) $(C_OBJS) -lpthread

$(S_EXE):$(S_OBJS)
	$(CC) -o $(S_EXE) $(S_OBJS)

$(T_EXE):$(T_OBJS)
	$(CXX) -o $(T_EXE) $(T_OBJS) -lpthread

$.o:$.c
	$(CC) -c $(CFLAGS) $< -o $@

$.o:$.cc
	$(CXX) -c $(CXXFLAGS) $< -o $@

.PHONY:clean
clean:
	rm -f $(C_EXE) $(C_OBJS) $(S_EXE) $(S_OBJS) $(T_EXE) $(T_OBJS)
