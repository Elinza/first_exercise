EXE = showtime
OBJS = showtime.o

$(EXE):$(OBJS)
	$(CC) -o $(EXE) $(OBJS)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f $(EXE) $(OBJS)
