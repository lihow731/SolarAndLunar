all: makedb.c transfer.c 
	gcc -o transfer transfer.c
	gcc -o makedb makedb.c
	gcc -o make24db make24db.c

clean: 
	rm -irf transfer makedb make24db
