#!/bin/bash
Smonth=(0 31 28 31 30 31 30 31 31 30 31 30 31)

if [ $1 ] ; then
	Y=$1
else
	Y=`date +%Y`
fi



for i in `seq 1 12`
do
	tmp=${Smonth[$i]}
	for j in `seq 1 $tmp`
	do
		echo -en "$Y $i $j \t"
		./transfer $Y $i $j
	done
done
