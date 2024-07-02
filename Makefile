CC = g++
CFLAGS = -Wall -g
APPLICATION = hegemony_server
INCLUDES = -Iinclude

all: $(APPLICATION)

$(APPLICATION): main.o
	$(CC) $(CFLAGS) -o $(APPLICATION) main.o

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

clean:
	rm -f *.o $(APPLICATION)
