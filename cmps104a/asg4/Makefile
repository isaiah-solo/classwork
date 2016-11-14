GCC        = g++ -g -O0 -Wall -Wextra -std=gnu++11
MKDEP      = g++ -MM -std=gnu++11
VALGRIND   = valgrind --leak-check=full --show-reachable=yes

MKFILE     = Makefile
DEPFILE    = Makefile.dep
SOURCES    = auxlib.cpp stringset.cpp lyutils.cpp astree.cpp oc.cpp symtable.cpp
HEADERS    = auxlib.h stringset.h lyutils.h astree.h symtable.h
CC         = ${CLGEN} ${CYGEN}
OBJECTS    = ${CC:.cpp=.o} ${SOURCES:.cpp=.o}
EXECBIN    = oc
SRCFILES   = ${HEADERS} ${SOURCES} ${MKFILE}
SMALLFILES = ${DEPFILE}
CHECKINS   = ${SRCFILES} ${SMALLFILES}
LISTING    = Listing.ps

LSOURCES   = scanner.l
YSOURCES   = parser.y
CLGEN      = yylex.cpp
HYGEN      = yyparse.h
CYGEN      = yyparse.cpp
LREPORT    = yylex.output
YREPORT    = yyparse.output
YACC       = bison -y

all : ${CYGEN} ${HYGEN} ${CLGEN} ${EXECBIN}

${CLGEN} : ${LSOURCES}
	flex --outfile=${CLGEN} ${LSOURCES} 2>${LREPORT}
	- grep -v '^ ' ${LREPORT}

${CYGEN} ${HYGEN} : ${YSOURCES}
	bison --defines=${HYGEN} --output=${CYGEN} ${YSOURCES}

${EXECBIN} : ${OBJECTS}
	${GCC} -o ${EXECBIN} ${OBJECTS} 

%.o : %.cpp
	${GCC} -c $<

ci :
	cid + ${CHECKINS}
	checksource ${CHECKINS}

clean :
	- rm *.o *.str *.tok *.ast yyparse.* yylex.* *.log

spotless : clean
	- rm ${EXECBIN} ${LISTING} ${LISTING:.ps=.pdf} ${DEPFILE} \
	     test.out test.err misc.lis

${DEPFILE} :
	touch yyparse.h
	${MKDEP} ${SOURCES} > ${DEPFILE}

dep :
	- rm ${DEPFILE}
	${MAKE} --no-print-directory ${DEPFILE}

include Makefile.dep

test : ${EXECBIN}
	${VALGRIND} ${EXECBIN} foo.oc 1>test.out 2>test.err

misc.lis : ${DEPFILE}
	catnv ${DEPFILE} >misc.lis

lis : misc.lis test
	mkpspdf ${LISTING} ${SRCFILES} misc.lis test.out test.err

submit :
	submit cmps104a-wm.f15 asg2 Makefile README *.cpp *.h \
	     parser.y scanner.l
