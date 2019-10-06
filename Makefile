.PHONY: all debug release clean

all: debug

debug:
	g++ -std=c++17 -Wall -pedantic -Iinclude/ -g src/*.cpp -o CntLang.out

release:
	g++ -std=c++17 -Wall -pedantic -Iinclude/ -O3 src/*.cpp -o CntLang.out

clean:
	rm -f CntLang.out
	rm -f out/*
