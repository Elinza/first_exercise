EXE = thread
OBJS = thread.o
C11_EXE = c11thread
C11_OBJS = c11thread.o
CXXFLAGS = -std=gnu++11

all: $(EXE) $(C11_EXE)


$(EXE):$(OBJS)
	$(CXX) -o $(EXE) $(OBJS) -lpthread

$(C11_EXE):$(C11_OBJS)
	$(CXX) -o $(C11_EXE) $(C11_OBJS) -lpthread

$.o:$.cc
	$(CXX) -c $(CXXFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f $(EXE) $(OBJS) $(C11_EXE) $(C11_OBJS)

