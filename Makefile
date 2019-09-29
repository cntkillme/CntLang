.PHONY: all clean

all:
	g++ -std=c++17 -Wall -pedantic -Iinclude/ -O3 src/*.cpp -o cntlang

clean:
	rm -f cntlang
