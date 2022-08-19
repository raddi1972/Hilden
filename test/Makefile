
lex.yy.c: scanner.ll
	flex scanner.ll

build: lex.yy.c main.cc tokens.hh Parser.hh Parser.cc Number.hh Number.cc AST.hh
	g++ lex.yy.c main.cc tokens.hh Parser.cc Number.cc -o hilden

clean:
	rm hilden lex.yy.c