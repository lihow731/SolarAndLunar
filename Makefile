objects = makedb.o make24db.o SolarAndLunar.o main.o 

all: transfer | create_db

transfer: $(objects)
	g++ -o transfer $(objects)

create_db:
	./transfer 
	./transfer 24

.c.o: makedb.c make24db.c SolarAndLunar.c db.h  makedb.h  SolarAndLunar.h

main.o: main.cpp SolarAndLunar.h makedb.h db.h
	g++ -c main.cpp

.PHONY: clean

clean: 
	rm -irf transfer *.o db2*
