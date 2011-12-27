#! /bin/bash

PATH=$PATH:/usr/local/bin
export PATH

cd /home/httpd/html/B-Free/mail-archive/mail3/all
./tohtml.sh

tar cvzf ../../mail3.tar.gz .

