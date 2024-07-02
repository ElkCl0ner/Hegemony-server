CC = g++
CFLAGS = -std=c++11 -Wall
APPLICATION = hegemony_server
INCLUDES = -Iinclude

OBJECTS = main.o pch.o

all: $(APPLICATION)

$(APPLICATION): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(APPLICATION) $(OBJECTS)

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

clean:
	rm -f *.o $(APPLICATION)

