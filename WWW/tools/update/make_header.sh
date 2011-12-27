#! /bin/bash

echo "<P>"
for year in 1998 1999 2000
do
    month_d=01;
    for month in Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec
    do
	if [ -s dat/$year$month_d.dat ]
	then
	    echo "<A HREF=\"$year$month_d.html\">"
	    echo "$year/$month "
	    echo "</A>"
	else
	    echo "$year/$month "
	fi
 	month_d=`expr $month_d + 1 |awk '{printf "%02d", $1}'`
    done
    echo "<BR>"
done
echo "<P>"
