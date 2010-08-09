#include <stdio.h>
#include "db.h"

#define SISLEAP(year)  ( ( ( year % 4  == 0 ) && ( year % 100 != 0 ) )  || ( year % 400 == 0 ) )

int SMONTHDAY[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

void ToSolar( int *y, int *m, int *d, int isleap)
{
	if ( ( *y < 1900 ) || ( *y > 2100 ) ) 
		return;
	if ( ( *m < 0 ) || ( *m > 12 ) )
		return;
	if ( ( *d < 1 ) || ( *d > 31 ) )
		return;

	int leap = stMOLTS[*y-1900].leap;
	int Syear = *y;
	int Smonth = 0 ;
	int Sday = 0 ;

	if ( leap == 0 )
	{
		Smonth = stMOLTS[*y-1900].Lmonth[*m - 1].month;
		Sday = stMOLTS[*y-1900].Lmonth[*m - 1].day + (*d) - 1;
	}
	else
	{
		if ( (  ( isleap == 1 ) && ( *m == leap ) ) || ( (  isleap == 0 )  && ( *m > leap )  ) )
		{
			Smonth = stMOLTS[*y-1900].Lmonth[*m ].month;
			Sday = stMOLTS[*y-1900].Lmonth[*m ].day + *d - 1;
		}
		else if ( ( isleap == 0 ) && (*m <= leap ))
		{	
			Smonth = stMOLTS[*y-1900].Lmonth[*m - 1].month;
			Sday = stMOLTS[*y-1900].Lmonth[*m - 1].day + *d - 1;
		}
		else
		{
			printf("No defined!!\n");
		}
	}
	if ( Sday > SMONTHDAY[Smonth] )
	{
		Sday -= SMONTHDAY[Smonth];
		Smonth ++;
		if ( Smonth >= 13 )
			Smonth = ( Smonth % 12 );
	}

	if ( ( *m > 10 ) && ( Smonth < 3 ) )
		Syear ++ ;

	*y = Syear;
	*m = Smonth;
	*d = Sday;
}

void ToLunar( int *y, int *m, int *d, int *l)
{
	if ( ( *y < 1900 ) || ( *y > 2100 ) ) 
		return;
	if ( ( *m < 1 ) || ( *m > 12 ) )
		return;
	if ( ( *d < 1 ) || ( *d > SMONTHDAY[*m] ) )
		return;

	int Lyear = *y;
	int Lmonth = 0;
	int Lday = 0;
	int Lleap = 0;
	int Ltemp = 0;	
	
	Lmonth = stMOLTS[*y-1900].Lmonth[*m - 1].month;
	Lday = stMOLTS[*y-1900].Lmonth[*m - 1].day + (*d) - 1;

	// example: 1902. 10. 31 => 1092. 10. 1
	// must to calculate the day of month of lunar then find correct day when Lday > 29.
	if ( Lday > 29 )
	{
		Lday = stMOLTS[*y-1900].Lmonth[*m].day - (*d) - 1;
		Ltemp = Lday;
	}
//	if ( Lday > SMONTHDAY[Smonth] )
//	{
//		Sday -= SMONTHDAY[Smonth];
//		Smonth ++;
//		if ( Smonth >= 13 )
//			Smonth = ( Smonth % 12 );
//	}
//
//	if ( ( *m > 10 ) && ( Smonth < 3 ) )
//		Syear ++ ;

	*y = Syear;
	*m = Smonth;
	*d = Sday;
}

void readDB()
{
	FILE *f;
	if ( ( f = fopen("db2", "rb") ) != NULL )
	{
//		printf("Open db2 successed!!\n");
		fread((void *)stMOLTS, sizeof(stMOLTS), 1 , f);	
		fclose(f);
	}
	else
	{
		printf("Open db2 file failure!\n");
	}

}

void findX_1()
{
	int i,j,k;
	int y,m,d;
	for ( i = 1900 ; i < 2100 ; i ++)
	{
		for ( j = 1 ; j <= 12 ; j++)
		{

			for ( k = 1 ; k <=30 ; k++)
			{
				y = i;
				m = j;
				d = k;
				ToSolar(&y,&m,&d, 0);
				if ( d == 1 )
					printf("%d %d %d => %d %d %d %d\n", y,m,d,i,j,k,0);
			}
			if ( stMOLTS[i - 1900].leap == j )
			{
				for ( k = 1 ; k <=30 ; k++)
				{
					y = i;
					m = j;
					d = k;
					ToSolar(&y,&m,&d, 1);
					if ( d == 1 )
						printf("%d %d %d => %d %d %d %d\n", y,m,d,i,j,k,1);
				}
			}
		}
	}
}

int main(int argc, int *argv[])
{
	readDB();
	int y, m, d, l;
	if ( argc >= 4 )
	{
		y = atoi(argv[1]);
		m = atoi(argv[2]);
		d = atoi(argv[3]);
		l = atoi(argv[4]);
//		printf("the Day is %d %d %d \n", y, m, d);
		ToSolar(&y, &m, &d, l);
	//	ToLunar(&y, &m, &d);
		
//		printf("the Day After transfered is %d %d %d \n", y, m, d);
		printf("%d %d %d\n", y, m, d);
		return 0;
	}
//	findX_1();
//	int i ; 
//	for ( i = 1 ; i <= 400 ; i ++ )
//		printf("%d is %s\n", i,SISLEAP(i)?"LEAP":"" );

}
