CC       = gcc
CFLAGS   = -g -O0 -Wall

TARGETS = interpreter

interpreter:
	$(CC) $(CFLAGS) -o interpreter interpreter.c 

all: $(TARGETS)

clean:
	rm -f $(TARGETS)