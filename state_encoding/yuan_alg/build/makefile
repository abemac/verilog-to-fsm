OBJS = Graph.o
CC = g++ -std=c++11
DEBUG = -g
LFLAGS = -Wall $(DEBUG)
CFLAGS = $(LFLAGS) -c

output: ../src/main.cpp
		$(CC) $(LFLAGS) ../src/main.cpp $(OBJS) -o ../run/yuan_alg
Graph.o: ../src/Graph.h ../src/Graph.cpp
		$(CC) $(CFLAGS) ../src/Graph.cpp
