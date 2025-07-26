CC=g++
CFLAGS=-c -Wall -std=c++17 -fPIC

LDFLAGS=-shared

TARGET=logger

LIBRARY=liblogger.so

all: $(TARGET)

$(TARGET): main.o log.o
	$(CC) main.o log.o -o $(TARGET) -L. -llogger -Wl,-rpath,.

main.o: main.cpp log.h
	$(CC) $(CFLAGS) main.cpp

log.o: log.cpp log.h
	$(CC) $(CFLAGS) log.cpp
	$(CC) $(LDFLAGS) -o $(LIBRARY) log.o

clean:
	rm -rf *.o $(TARGET) $(LIBRARY)