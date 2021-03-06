CC = g++
CCFLAGS = -g -W -Wall

FLEX = flex
FLEX_OPTS = -Pzp

BISON = bison
BISON_OPTS = -t -pzp

OBJS = Absyn.o Lexer.o Parser.o Printer.o

.PHONY: clean distclean

all: Testzp

clean:
	rm -f *.o Testzp zp.aux zp.log zp.pdf zp.dvi zp.ps zp

distclean: clean
	rm -f Absyn.C Absyn.H Test.C Parser.C Parser.H Lexer.C Skeleton.C Skeleton.H Printer.C Printer.H Makefile zp.l zp.y zp.tex 

Testzp: ${OBJS} Test.o
	@echo "Linking Testzp..."
	${CC} ${CCFLAGS} ${OBJS} Test.o -o Testzp

Absyn.o: Absyn.C Absyn.H
	${CC} ${CCFLAGS} -c Absyn.C

Lexer.C: zp.l
	${FLEX} -oLexer.C zp.l

Parser.C: zp.y
	${BISON} zp.y -o Parser.C

Lexer.o: Lexer.C Parser.H
	${CC} ${CCFLAGS} -c Lexer.C

Parser.o: Parser.C Absyn.H
	${CC} ${CCFLAGS} -c Parser.C

Printer.o: Printer.C Printer.H Absyn.H
	${CC} ${CCFLAGS} -c Printer.C

Skeleton.o: Skeleton.C Skeleton.H Absyn.H
	${CC} ${CCFLAGS} -c Skeleton.C

Test.o: Test.C Parser.H Printer.H Absyn.H
	${CC} ${CCFLAGS} -c Test.C

