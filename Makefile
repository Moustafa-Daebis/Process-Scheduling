CC = g++ -g
all: lab4
lab4.o: lab4.cpp
	$(CC) -c lab4.cpp
lab4:  lab4.o
	$(CC) -o lab4 lab4.o -ll

