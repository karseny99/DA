CC=g++
FLAGS=-std=c++20 -pedantic -Wextra -Wno-unused-variable 
OUTPUT=lab1

all: sort.o lab1.cpp
	$(CC) $(FLAGS) sort.o lab1.cpp -o $(OUTPUT)
sort.o: sort.cpp sort.hpp
	$(CC) $(FLAGS) -c sort.cpp
benchmark: sort.o benchmark.cpp
	$(CC) $(FLAGS) sort.o benchmark.cpp -o benchmark
clean:
	rm *.o $(OUTPUT)
