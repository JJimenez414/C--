all: cmm

cmm_parser.tab.c cmm_parser.tab.h: cmm_parser.y
	@echo "Note: Bison's 1 shift/reduce conflict is expected and harmless."
	bison -d cmm_parser.y

lex.yy.c: cmm_lexer.l cmm_parser.tab.h strpool.h
	flex cmm_lexer.l

ast.o: ast.c ast.h
	gcc -c ast.c

strpool.o: strpool.c strpool.h
	gcc -c strpool.c

process.o: process.c process.h ast.h
	gcc -c process.c

cmm: cmm_parser.tab.c lex.yy.c ast.o process.o strpool.o
	gcc -o cmm cmm_parser.tab.c lex.yy.c ast.o process.o strpool.o -lfl

clean:
	@rm -f cmm cmm_parser.tab.* lex.yy.c ast.o process.o strpool.o
