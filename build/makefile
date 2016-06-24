OBJS = Wire.o Module.o Processor.o And.o Not.o Or.o Nand.o Nor.o Dff.o Simulator.o
CC = g++ -std=c++11
DEBUG = -g
LFLAGS = -Wall $(DEBUG)
CFLAGS = $(LFLAGS) -c

output: $(OBJS) ../src/main.cpp
		$(CC) $(LFLAGS) ../src/main.cpp $(OBJS) -o ../run/linux_output
Wire.o: ../src/Wire.h ../src/Wire.cpp
		$(CC) $(CFLAGS) ../src/Wire.cpp
Processor.o: ../src/Processor.h ../src/Processor.cpp ../src/Wire.h $(wildcard ../src/modules/*.h)
		$(CC) $(CFLAGS) ../src/Processor.cpp
Module.o: ../src/Module.h ../src/Module.cpp ../src/Wire.h
		$(CC) $(CFLAGS) ../src/Module.cpp
Not.o: ../src/modules/Not.h ../src/modules/Not.cpp ../src/Module.h
		$(CC) $(CFLAGS) ../src/modules/Not.cpp
And.o: ../src/modules/And.h ../src/modules/And.cpp ../src/Module.h
		$(CC) $(CFLAGS) ../src/modules/And.cpp
Or.o: ../src/modules/Or.h ../src/modules/Or.cpp ../src/Module.h
		$(CC) $(CFLAGS) ../src/modules/Or.cpp
Nand.o: ../src/modules/Nand.h ../src/modules/Nand.cpp ../src/Module.h
		$(CC) $(CFLAGS) ../src/modules/Nand.cpp
Nor.o: ../src/modules/Nor.h ../src/modules/Nor.cpp ../src/Module.h
		$(CC) $(CFLAGS) ../src/modules/Nor.cpp
Dff.o: ../src/modules/Dff.h ../src/modules/Dff.cpp ../src/Module.h
		$(CC) $(CFLAGS) ../src/modules/Dff.cpp
Simulator.o: ../src/Simulator.h ../src/Simulator.cpp ../src/Wire.h $(wildcard ../src/modules/*.h)
		$(CC) $(CFLAGS) ../src/Simulator.cpp
clean:
	rm *.o ../run/linux_output
