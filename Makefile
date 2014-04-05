CC=g++
CFLAGS=-c -Wall

all: clean parallel

parallel: main.o global.o painter.o helper.o clearing_helper.o filling_helper.o
	$(CC) main.o global.o painter.o helper.o filling_helper.o clearing_helper.o -o parallel -lpthread

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

helper.o: helper.cpp
	$(CC) $(CFLAGS) helper.cpp

clearing_helper.o: clearing_helper.cpp
	$(CC) $(CFLAGS) clearing_helper.cpp -o clearing_helper.o

filling_helper.o: filling_helper.cpp
	$(CC) $(CFLAGS) filling_helper.cpp -o filling_helper.o

painter.o: painter.cpp
	$(CC) $(CFLAGS) painter.cpp

global.o: global.cpp
	$(CC) $(CFLAGS) global.cpp

clean:
	rm -rf *o parallel

run:
	./parallel $3