# Planning to add tests to check performance
SRC=src/client.cpp
HEADER=include/bplus.h
CXX=g++
CPPFLAGS = -Wall -std=c++20 -pedantic -I ./include/

all: a.out

a.out: $(SRC) $(HEADER)
	@$(CXX) $(CPPFLAGS) $(SRC)

debug: $(SRC)
	$(CXX) $(CPPFLAGS) -DDEBUG=1 $(SRC) -o debug.out

.PHONY: clean

clean:
	@rm -f a.out debug.out