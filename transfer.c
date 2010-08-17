#include <stdio.h>
#include "SolarAndLunar.h"
#include "db.h"
#define SISLEAP(year)  ( ( ( year % 4  == 0 ) && ( year % 100 != 0 ) )  || ( year % 400 == 0 ) )

static struct MappingOfLunarToSolar stMOLTS[200];

static int SMONTHDAY[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

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
//		else
//		{
//			printf("No defined!!\n");
//		}
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

	int leap   = stMOLTS[*y-1900].leap;
	int Lmonth = 0;
	int Lday   = 0;
	int LtempL = stMOLTS[*y-1900].Smonth[*m - 1].leap;
	int LtempD = 1;	
	int LtempM; 
	int LtempY;
	int tmp;	
	int tmp2;	
	
	Lday   = stMOLTS[*y-1900].Smonth[*m - 1].day + (*d) - 1;
	Lmonth = stMOLTS[*y-1900].Smonth[*m - 1].month;

	// example: 1902. 10. 31 => 1902. 10. 1
	// must to calculate the day of month of lunar then find correct day when Lday > 29.
	if ( Lday > 29 )
	{
		// if next month is leap month..
		if ( leap != 0 && leap == Lmonth && LtempL == 0  )
		{
			LtempM = tmp = stMOLTS[*y-1900].Smonth[*m - 1].month;	
			LtempY = (LtempM > *m)?(*y-1):*y;
			tmp2 = LtempL;
			LtempL = 1;
		}
		else
		{
			LtempM = tmp = ( stMOLTS[*y-1900].Smonth[*m - 1].month % 12 ) + 1;	
			LtempY = (LtempM > *m)?(*y-1):*y;
			tmp2 = LtempL;
			LtempL = 0;
		}
		ToSolar( &LtempY, &LtempM, &LtempD, LtempL); // 1902.9.1

		if ( LtempM == *m )
		{
			if ( LtempD > *d ) 
			{
				Lday = 30;   // day 30
				LtempL = tmp2;
			}
			else
			{
				Lday = 1 + ( *d - LtempD );
				Lmonth = tmp ;
			}
		}
		else if ( LtempM > *m )
		{
			Lmonth = ( Lmonth % 12 ) + 1 ;
			LtempD = 1;

			// if next month is leap month.. (there are some bug, here!!)
			// example:How to prcess 4 5 5(leap)?
			if ( leap != 0 && leap == Lmonth && LtempL == 0  )
			{
				LtempM = stMOLTS[*y-1900].Smonth[*m - 1].month;	
				LtempY = ((LtempM > *m)?(*y-1):*y ); 
				LtempL = 1;
			}
			else
			{
				LtempM = (( stMOLTS[*y-1900].Smonth[*m - 1].month % 12 ) + 2 );
				LtempY = ((LtempM > *m)?(*y-1):*y ); 
				LtempL = 0;
			}
			ToSolar( &LtempY, &LtempM, &LtempD, LtempL); // 1902.10.1



			if ( LtempD <= *d && LtempM == *m )
			{
				Lday = 1 + ( *d - LtempD );	
			}
		}
	}
	if ( Lmonth > *m )
		*y = *y - 1;
	*m = Lmonth;
	*d = Lday;

	if ( leap == Lmonth && LtempL == 1 )
		*l = 1;
	else 
		*l = 0;
	
	
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

//void findX_1()
//{
//	int i,j,k;
//	int y,m,d;
//	for ( i = 1900 ; i < 2100 ; i ++)
//	{
//		for ( j = 1 ; j <= 12 ; j++)
//		{
//
//			for ( k = 1 ; k <=30 ; k++)
//			{
//				y = i;
//				m = j;
//				d = k;
//				ToSolar(&y,&m,&d, 0);
//				if ( d == 1 )
//					printf("%d %d %d => %d %d %d %d\n", y,m,d,i,j,k,0);
//			}
//			if ( stMOLTS[i - 1900].leap == j )
//			{
//				for ( k = 1 ; k <=30 ; k++)
//				{
//					y = i;
//					m = j;
//					d = k;
//					ToSolar(&y,&m,&d, 1);
//					if ( d == 1 )
//						printf("%d %d %d => %d %d %d %d\n", y,m,d,i,j,k,1);
//				}
//			}
//		}
//	}
//}

//int main(int argc, int *argv[])
//{
//	readDB();
//	int y, m, d, l;
//	if ( argc >= 5 )
//	{
//		y = atoi(argv[1]);
//		m = atoi(argv[2]);
//		d = atoi(argv[3]);
//		l = atoi(argv[4]);
//		ToSolar(&y, &m, &d, l);
//		
//		printf("%d %d %d \n", y, m, d);
//		return 0;
//	}
//	else if ( argc == 4 )
//	{
//		y = atoi(argv[1]);
//		m = atoi(argv[2]);
//		d = atoi(argv[3]);
//		ToLunar(&y, &m, &d, &l);
//		printf("%d %d %d %s\n", y, m, d, (l != 0)?"leap":"");
//
//
//	}
////	findX_1();
////	int i ; 
////	for ( i = 1 ; i <= 400 ; i ++ )
////		printf("%d is %s\n", i,SISLEAP(i)?"LEAP":"" );
//
//}
