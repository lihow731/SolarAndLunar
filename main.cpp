#include <cstdlib>
#include <iostream>
#include <stdio.h>
extern "C" {
#include "SolarAndLunar.h"
#include "makedb.h"
}

using namespace std;

int main(int argc, char *argv[])
{

	int y, m, d, l;
	if ( argc >= 5 )
	{
      	readDB();
		y = atoi(argv[1]);
		m = atoi(argv[2]);
		d = atoi(argv[3]);
		l = atoi(argv[4]);
		ToSolar(&y, &m, &d, l);
		
		printf("%d %d %d \n", y, m, d);
		return 0;
	}
	else if ( argc == 4 )
	{
	    readDB();
		y = atoi(argv[1]);
		m = atoi(argv[2]);
		d = atoi(argv[3]);
		ToLunar(&y, &m, &d, &l);
		printf("%d %d %d %s\n", y, m, d, (l != 0)?"leap":"");


	}
	else if ( argc == 1 )
	{
		make24SolarTermsDB();
		save2file24();
	}
	else
	{
		makeLunarDB();
		makeSolarDB();
		save2file();   
	}
    	
//	system("PAUSE");
	return EXIT_SUCCESS;
}
