#!/bin/sh
#
#	pack2dos.sh - ソースファイルを DOS 形式のファイルに変換してまとめる。
#
#	書式：
#		pack2dos.sh <pack-file>
#

TMPDIR=/tmp/bfree
TMPFILE=/tmp/convert.tmp

function convert
{
	nkf -x $1 > $TMPFILE
	unix2dos < $TMPFILE > $2
}


mkdir /tmp/bfree
cd $TMPDIR

cvs checkout btron/src btron/README-source.EUC

nkf -x btron/README-source.EUC > btron/README-source.SJIS

find btron -print |
while read line
do
	shortnames $line 
done > TRANS.TBL

cat TRANS.TBL | awk '{ printf "%s\n%s\n", $2, $3 }' | 
while read src
do
	read dest
	if [ -d $src ]
	then
		mkdir $dest
	elif [ -f $src ]
	then
		convert $src $dest
	fi
done

tar cvzf  $1 BTRON TRANS.TBL

exit 0
