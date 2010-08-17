#include <stdio.h>
#include <string.h>
#include "makedb.h"
#include "db.h"

static struct MappingOfLunarToSolar stMOLTS[200];

int makeLunarDB()
{
	int Y, M, D, LM; 
	int lastM = 0;;
	int lastY = 1900;
	int isLeap = 0;
	unsigned int data = 0;
	FILE *f;
	int count = 0;
	int mCount = 0;
	memset( (void *) stMOLTS, 0x0, sizeof(stMOLTS));
	f = fopen("Lunar2Solar.txt","r");
	if ( f != 0)
	{
		printf("open file successed! sizeof(int)= %d\n", sizeof(int));
		while ( ! feof(f) )
		{
			fscanf(f,"%d%d%d%d", &M, &D, &Y, &LM);		
			//printf("%d %d %d %d \t", Y, M , D, LM);

			if ( lastM == LM ) 
			{
				isLeap = 1;
				stMOLTS[Y-1900].leap = LM;
			}
			else
				isLeap = 0;

			if ( LM == 1 )
				mCount = 0;

				
			lastM = LM;
			lastY = Y;
			
			if ( ( LM > M ) && ( Y > 1900 ) )
			{
				stMOLTS[Y-1900-1].Lmonth[mCount].month=M;
				stMOLTS[Y-1900-1].Lmonth[mCount].day=D;
			}
			else
			{
				stMOLTS[Y-1900].Lmonth[mCount].month=M;
				stMOLTS[Y-1900].Lmonth[mCount].day=D;
			}
			stMOLTS[Y-1900].year = Y - 1;
			stMOLTS[Y-1900].year = Y;
			mCount ++ ;
			count ++ ;
		}
		fclose(f);
	}
#ifdef DEBUG
	int i = 0 ; 
	int j = 0 ; 
	for ( i = 0; i < 200 ; i ++)
	{
		printf("year = %d, leap = %d\n",stMOLTS[i].year ,stMOLTS[i].leap );
		for ( j = 0 ; j < 13 ; j ++ )
			printf("\tmonth = %d, day = %d\n", stMOLTS[i].Lmonth[j].month, stMOLTS[i].Lmonth[j].day);
	}
#endif // DEBUG //
}

int makeSolarDB()
{
	FILE *f;
	if ( ( f = fopen("Solar2Lunar.txt","r")) != NULL )
	{
		int SY, SM, SD, LY, LD, LM, LL;
		char A[2];
		while ( ! feof(f) )
		{
			fscanf(f,"%d%d%d%s%d%d%d%d",&SY,&SM,&SD,A,&LY,&LM,&LD,&LL);
			stMOLTS[SY-1900].Smonth[SM-1].month=LM;
			stMOLTS[SY-1900].Smonth[SM-1].day=LD;
			stMOLTS[SY-1900].Smonth[SM-1].leap=LL;
		}
		
		fclose(f);
	}
#ifdef DEBUG
	int i = 0 ; 
	int j = 0 ; 
	for ( i = 0; i < 200 ; i ++)
	{
		printf("year = %d, leap = %d\n",stMOLTS[i].year ,stMOLTS[i].leap );
		for ( j = 0 ; j < 12 ; j ++ )
			printf("\tmonth = %d, day = %d, leap = %d\n", stMOLTS[i].Smonth[j].month, stMOLTS[i].Smonth[j].day, stMOLTS[i].Smonth[j].leap);
	}
#endif //DEBUG//
};

void save2file()
{
	FILE *f2;
	f2 = fopen("db2","wb");
	if ( f2 != NULL )
	{
		printf("Open db2 successed!! sizeof(stMOLTS)=%d\n", sizeof(stMOLTS));
		fwrite((void *)stMOLTS, sizeof(stMOLTS), 1, f2);
		fclose(f2);
	}

}

//int main ( void )
//{
//	makeLunarDB();
//	makeSolarDB();
//	save2file();
//	return 0;
//}
