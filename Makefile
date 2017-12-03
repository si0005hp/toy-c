CC       = gcc
CFLAGS   = -g -O0 -Wall
LEX      = lex
YACC     = yacc

TARGETS = main

main: main.c lex.yy.c y.tab.c
	$(CC) $(CFLAGS) -o main main.c lex.yy.c y.tab.c -ll
	
lex.yy.c: lex.l y.tab.h
	$(LEX) lex.l
	
y.tab.h: parse.y
	$(YACC) -d parse.y

all: $(TARGETS)

test: $(TARGETS)
	bundle exec rspec

clean:
	rm -f $(TARGETS) *.yy.c *.tab.* *.output
