all: makedb2.c transfer.c 
	gcc -o transfer transfer.c
	gcc -o makedb makedb.c

clear: 
	rm -irf transfer makedb
