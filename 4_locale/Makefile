EXE = sub202
OBJS = sub202.o

all:$(EXE)

$(EXE): $(OBJS)
	$(CC) -o $(EXE) $(OBJS)

%.o:%.c
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY : clean
clean :
	rm -f $(EXE) $(OBJS)
