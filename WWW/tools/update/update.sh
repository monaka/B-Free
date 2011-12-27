#! /bin/bash

PATH=$PATH:/usr/local/bin
export PATH

if [ ! -d dat ]
then
    mkdir dat
fi
if [ ! -d index ]
then
    mkdir index
fi

for year in 1998 1999 2000
do
    month_d=01;
    for month in Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec
    do
	for i in `/usr/local/bin/imgrep --expression="body='Changed by' && date=$month $year" +bfree/update `
	do
	    imcat +bfree/update $i | nkf -e | ./getmail.pl 
	done > dat/$year$month_d.dat
 	month_d=`expr $month_d + 1 |awk '{printf "%02d", $1}'`
    done
done

sh make_header.sh > header.html
sh update_format.sh


