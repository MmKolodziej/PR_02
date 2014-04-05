CC=g++
CFLAGS=-c -Wall

all: parallel

parallel: main.o painter.o helper.o
	$(CC) main.o painter.o helper.o -o parallel

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

clearing_helper.o: clearing_helper.cpp
	$(CC) $(CFLAGS) clearing_helper.cpp -o helper

filling_helper.o: filling_helper.cpp
	$(CC) $(CFLAGS) filling_helper.cpp -o helper.o

helper.o: helper.cpp
	$(CC) $(CFLAGS) helper.cpp

painter.o: painter.cpp
	$(CC) $(CFLAGS) painter.cpp

clean:
	rm -rf *o parallel
