objects = makedb.o make24db.o SolarAndLunar.o main.o 

all: $(objects)
	g++ -o transfer $(objects)

.c.o: makedb.c make24db.c SolarAndLunar.c db.h  makedb.h  SolarAndLunar.h

main.o: main.cpp SolarAndLunar.h makedb.h db.h
	g++ -c main.cpp
clean: 
	rm -irf transfer *.o db2*
