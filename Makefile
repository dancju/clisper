main:
	g++ src/main.cpp -o bin/main --std=c++11

test: main
	bin/main < test.lisp
