CLIENT=src/client.cpp
TEST=src/tests.cpp
HEADER=include/bplus.h
CXX=g++
CPPFLAGS = -O3 -Wall -std=c++20 -g -pedantic -I include/

all: a.out

a.out: $(CLIENT) $(HEADER)
	@$(CXX) $(CPPFLAGS) $(CLIENT)

debug: $(CLIENT) $(HEADER)
	$(CXX) $(CPPFLAGS) -g -DDEBUG=1 $(CLIENT) -o debug.out

test: $(HEADER) $(TEST)
	$(CXX) $(CPPFLAGS) $(TEST) -o test.out

.PHONY: clean

clean:
	@rm -f a.out debug.out test.out