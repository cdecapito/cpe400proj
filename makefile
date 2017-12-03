CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

main : main.o 
	$(CC) $(LFLAGS) main.o -o main

main.o : main.cpp objects.cpp functions.cpp
	$(CC) $(CFLAGS) main.cpp objects.cpp functions.cpp

clean: 
	\rm *.o main
