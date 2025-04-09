all: cmm

cmm_parser.tab.c cmm_parser.tab.h: cmm_parser.y
	@echo "Note: Bison's 1 shift/reduce conflict is expected and harmless."
	bison -d cmm_parser.y

lex.yy.c: cmm_lexer.l cmm_parser.tab.h
	flex cmm_lexer.l

cmm: cmm_parser.tab.c lex.yy.c
	gcc -o cmm cmm_parser.tab.c lex.yy.c -lfl

clean:
	@rm -f cmm cmm_parser.tab.* lex.yy.c
