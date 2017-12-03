CC       = gcc
CFLAGS   = -g -O0 -Wall

TARGETS = interpreter lexer

interpreter:
	$(CC) $(CFLAGS) -o interpreter interpreter.c 
	
lexer: lex.yy.c
	$(CC) $(CFLAGS) -o lexer lex.yy.c -ll
	
lex.yy.c: lex.l
	lex lex.l

all: $(TARGETS)

clean:
	rm -f $(TARGETS)