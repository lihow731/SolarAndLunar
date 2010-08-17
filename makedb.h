#ifndef SOLARANDLUNAR_MAKEDB
#define SOLARANDLUNAR_MAKEDB

struct SOLARTERMS{
	int  month  ;
	int  day    ;
	int  hour   ;
	int  minute ;
	int  second ;
};

struct SOLARTERM24 {
	int year;
	struct SOLARTERMS sts[24];
};

// makedb
int makeLunarDB();
int makeSolarDB();
void save2file();

// makedb24
void make24SolarTermsDB();
void save2file24();
void search(int, int, int, int);

#endif // SOLARANDLUNAR_MAKEDB //
