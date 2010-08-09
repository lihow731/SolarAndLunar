all: makedb.c transfer.c 
	gcc -o transfer transfer.c
	gcc -o makedb makedb.c

clean: 
	rm -irf transfer makedb
