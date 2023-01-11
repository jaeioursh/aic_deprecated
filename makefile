#cppyy

default: test.cpp
	g++ -o test.exe test.cpp
	./test.exe