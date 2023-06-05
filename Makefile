OPTIONS=-O2

all : tp4

tp4 : tp4.cpp journal.o date.o
	g++ ${OPTIONS} -o tp4 tp4.cpp journal.o date.o
	
date.o : date.cpp date.h
	g++ ${OPTIONS} -c -o date.o date.cpp

journal.o : journal.cpp journal.h date.h arbreavl.h
	g++ ${OPTIONS} -c -o journal.o journal.cpp

testdate : testdate.cpp date.o
	g++ $(OPTIONS) -o testdate testdate.cpp date.o

clean:
	rm -rf tp4 *~ *.o *.gch

