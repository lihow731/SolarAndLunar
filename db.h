struct Sday {
	int month;
	int day;
};

struct Lday {
	int month;
	int day;
	int leap;
};

struct MappingOfLunarToSolar {
	int year;
	struct Lday Smonth[12]; // From Solar to Lunar
	struct Sday Lmonth[13]; // put the day of Solar. Mean What day the lunar x.1 is?
	int leap;
};

struct MappingOfLunarToSolar stMOLTS[200];
