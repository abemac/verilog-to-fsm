OBJS = Graph.o Path.o
CC = g++ -std=c++11
DEBUG = -g
LFLAGS = -Wall $(DEBUG)
CFLAGS = $(LFLAGS) -c

output: ../src/main.cpp $(OBJS)
		$(CC) $(LFLAGS) ../src/main.cpp $(OBJS) -o ../run/yuan_alg
Graph.o: ../src/Graph.h ../src/Graph.cpp
		$(CC) $(CFLAGS) ../src/Graph.cpp
Path.o: ../src/Path.h ../src/Path.cpp
		$(CC) $(CFLAGS) ../src/Path.cpp
