#!/bin/bash
# CMPT 332 -- Fall 2017
# Assignment 1
# Derek Perrin 		dmp450 11050915
# Dominic McKeith 	dom258 11184543

OS=`uname`
PROG=part$1
INT=[1-9][0-9]*


checkArgs(){
	if ! [[ $# == 3 && $@ =~ ^$INT\ $INT\ $INT$ ]]; then
		echo "Invalid arguments"
		continue
	fi
}

main(){
	while read -p "> " line; do
		checkArgs $line
		./$PROG $line
	done
}
	
invalidInput(){
	echo "Error: invalid input"
	exit 1
}
# check that the number of parameters is 1
if [ $# -eq 1 ] ; then
	# check that the OS matches what part the user is trying to run
	if [[ $OS =~ MINGW* && $1 == "A1" ]]; then
		main
	elif ! [[ $OS =~ MINGW* ]] && [[ $1 =~ A[2-4] ]]; then
		main
	else
		invalidInput
	fi
else
	invalidInput
fi

exit 0

