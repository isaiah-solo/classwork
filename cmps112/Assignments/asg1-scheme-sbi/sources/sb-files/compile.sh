#!/bin/sh -x
# $Id: compile.sh,v 1.7 2012-01-19 17:13:09-08 - - $
sbirdir=../sbir-files
sbtran=../translator/sbtran
cid + $0
if [ ! -d $sbirdir ]
then
   mkdir $sbirdir
fi
for sbfile in *.sb
do 
   cid + $sbfile
   $sbtran $sbfile >../sbir-files/${sbfile}ir
done
mkpspdf ../sb-files.ps *.sb $0
