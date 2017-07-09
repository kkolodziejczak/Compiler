CC=gcc
LEX=flex
YACC=bison
CPP=g++

SRCS = 	Utils.cpp \
				VariableInfo.cpp \
				Generator.cpp \
				Manager.cpp 
		
OBJECTS = $(SRCS:.cpp=.o)

all:	leks

leks:	def.tab.o lex.yy.o $(OBJECTS)
			$(CPP) lex.yy.o def.tab.o $(OBJECTS) -o leks -ll -std=c++14

lex.yy.o:	lex.yy.c
			$(CC) -c lex.yy.c

lex.yy.c:	zMoje.l
			$(LEX) zMoje.l

def.tab.o:	def.tab.cc
			$(CPP) -c def.tab.cc -std=c++14

def.tab.cc:	def.yy
			$(YACC) -d def.yy

Utils.o:	Utils.cpp
			$(CPP) -c Utils.cpp -std=c++14

VariableInfo.o:	VariableInfo.cpp
			$(CPP) -c VariableInfo.cpp -std=c++14

Generator.o:	Generator.cpp
			$(CPP) -c Generator.cpp -std=c++14

Manager.o:	Manager.cpp
			$(CPP) -c Manager.cpp -std=c++14


clean:
	rm *.o leks
