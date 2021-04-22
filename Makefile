a.out: client.cpp
	g++ client.cpp

.PHONY: clean

clean:
	rm a.out
