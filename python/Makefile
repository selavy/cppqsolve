CC = gcc
CFLAGS = 
INC = `/usr/bin/python2.7-config --cflags`
PYTHON = `/usr/bin/python2.7-config --ldflags`

OBJS = main.o

mypy: $(OBJS)
	$(CC) $(CFLAGS) $(INC) -o mypy $(OBJS) $(PYTHON)
main.o: main.c	
	$(CC) $(CFLAGS) $(INC) -c main.c $(PYTHON)
.PHONY: clean
clean:
	rm -rf $(OBJS) mypy
