#include <stdio.h>
#include <string.h>
#include <time.h>
#include "makedb.h"
#include "SolarAndLunar.h"

//#define DEBUG

struct SOLARTERM24 ST[200];

int SMONTHDAY[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

void make24SolarTermsDB()
{
	//char *SolarTerms[]={""};
	FILE *f;
	char SolarTerms[5];
	char Month[4];
	char Date[9];
	char Meridiem[3];
	int  term   = 0;
	int  year   = 0;
	int  month  = 0;
	int  day    = 0;
	int  hour   = 0;
	int  minute = 0;
	int  second = 0;
	
	memset((void *)&SolarTerms, 0x0, sizeof(SolarTerms));
	memset((void *)&Month, 0x0, sizeof(Month));
	memset((void *)&Date, 0x0, sizeof(Date));
	memset((void *)&Meridiem, 0x0, sizeof(Meridiem));

	f = fopen("24solarterms.txt","r");
	if ( f != NULL  )
	{
		while ( ! feof(f) )	
		{
			fscanf(f,"%d%d%d%d%d%d%d",&term, &year, &month,&day,&hour, &minute, &second);
#ifdef DEBUG			
			printf("%d %d %d %d %d %d %d\n",term, year, month, day, hour, minute, second);
#endif // DEBUG //
			ST[year - 1900].year = year;
			ST[year - 1900].sts[term-1].month = month;
			ST[year - 1900].sts[term-1].day = day;
			ST[year - 1900].sts[term-1].hour = hour;
			ST[year - 1900].sts[term-1].minute = minute;
			ST[year - 1900].sts[term-1].second = second;
			
		}
		fclose(f);
	}
#ifdef DEBUG
	int i,j;
	for ( i = 0 ; i < 200 ; i ++ )
	{
		printf("Year = %d\n",i + 1900);
		for ( j = 0 ; j < 24 ; j ++ )
		{
			printf("\tterm = %d",j + 1);
			printf("\t%d/%d %d:%d:%d\n",ST[i].sts[j].month, ST[i].sts[j].day, ST[i].sts[j].hour, ST[i].sts[j].minute, ST[i].sts[j].second);
		}	
	}
#endif  // DEBUG //
	
}

void save2file24()
{
	FILE *f2;
	f2 = fopen("db24","wb");
	if ( f2 != NULL )
	{
#ifdef DEBUG			
		printf("Open db24 successed!! sizeof(stMOLTS)=%d\n", sizeof(ST));
#endif // DEBUG //
		fwrite((void *)ST, sizeof(ST), 1, f2);
		fclose(f2);
	}

}

// input is Gregorian.
void search(int y, int m, int d, int h)
{

	if ( y < 1900 || y > 2100 )
		return;
	if ( m < 1 || m > 12 )
		return;
	if ( d < 1 || d > SMONTHDAY[m] )
		return;
	if ( h < 0 || h > 23 )
		return;

    		
	int i, j;
	int after, before;
	int afterY = y;
	int beforeY = y;
	int solarTerm = 0;
	int c = -1;

//        case 1
//   n  ------------------
// 	  case *
//  n+1 ------------------
//        case 2

	struct SOLARTERMS sts1, sts2;
	for ( i = 0 ; i < 12  ; i ++ )
	{
		if ( ST[y-1900].sts[i*2].month == m )
		{
			solarTerm = i;
			if ( ST[y-1900].sts[i*2].day < d )
				c = 1;
			else if ( ST[y-1900].sts[ i * 2 + 1].day > d )
				c = 2;
			else if ( ST[y-1900].sts[i * 2].day <= d  && d <= ST[y-1900].sts[i * 2 + 1].day )
			{
				if ( h < ST[y-1900].sts[i*2].hour )
				{
					c = 1;
				}
				else if ( ST[y-1900].sts[i*2].hour <= h )
					c = 2;
//				if ( ST[y-1900].sts[i * 2].hour  <= h  && h <= ST[y-1900    ].sts[i * 2 + 1].minute )
//				{
//					if ( mi < ST[y-1900].sts[i*2].minute  )
//						c = 1;
//					if ( ST[y-1900].sts[i*2].minute <= mi )
//						c = 2;
//				}

			}
		}
	}

	switch ( c )
	{
		case 1:
			if ( solarTerm  == 0 ) 
			{
				if ( y == 1900 )
					return;
				afterY = y - 1;
				after = 23;
			}
			else
				after = solarTerm  * 2 - 1;

			before = solarTerm  * 2;
			break;
		case 2:
			after = solarTerm  * 2 + 1;
			if ( solarTerm  == 11 )
			{
				if ( y = 2099 )
					return;
				beforeY = y + 1;
				before = 0;
			}
			else
				before = solarTerm  * 2 + 2;
			break;
		default:
			after = solarTerm  * 2;
			before = solarTerm  * 2 + 1;
			break;
	}

	memcpy((void *)&sts1, (void *)&(ST[afterY-1900].sts[after])  , sizeof(sts1));
	memcpy((void *)&sts2, (void *)&(ST[beforeY-1900].sts[before])  , sizeof(sts2));
	printf("\t%d/%d/%d %d:%d:%d\n",afterY,sts1.month, sts1.day, sts1.hour, sts1.minute, sts1.second);
	printf("\t%d/%d/%d %d:%d:%d\n",beforeY,sts2.month, sts2.day, sts2.hour, sts2.minute, sts2.second);

}

//int main ( int argc, char *argv[] )
//{
//	make24SolarTermsDB();
//	save2file24();
//
//	int y, m, d, h;
//	if ( argc == 5 )
//	{
//		y = atoi(argv[1]);
//		m = atoi(argv[2]);
//		d = atoi(argv[3]);
//		h = atoi(argv[4]);
//		search(y, m, d, h);
//	}
//
//	return 0;
//}
