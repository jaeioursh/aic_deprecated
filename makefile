#cppyy

default: test.cpp
	g++ -o test.exe test.cpp
	./test.exe

test: test.cpp
	g++ -o test.exe test.cpp -g
	gdb ./test.exe