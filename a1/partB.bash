#!/bin/bash
# CMPT 332 -- Fall 2017
# Assignment 1
# Derek Perrin 		dmp450 11050915
# Dominic McKeith 	dom258 11184543

OS=`uname`
echo $OS

if [ $# -eq 1 ] && [[ "$1" =~ "A"[1-4] ]]; then
	echo hdsad
else
	echo "Error: invalid input."
	exit 1
fi



while read line; do
	echo "> $line"
done

exit 0