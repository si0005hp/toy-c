CC       = gcc
CFLAGS   = -g -O0 -Wall

TARGETS = main

main: lex.yy.c y.tab.c
	$(CC) $(CFLAGS) -o main lex.yy.c y.tab.c -ll
	
lex.yy.c: lex.l y.tab.h
	lex lex.l
	
y.tab.h: parse.y
	yacc -d parse.y

all: $(TARGETS)

test: $(TARGETS)
	bundle exec rspec

clean:
	rm -f $(TARGETS) *.yy.c *.tab.* *.output
