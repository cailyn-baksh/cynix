#!/bin/bash
# updates build number by one when called

build=74

# write to stdout then increment
if [[ $# == 1 ]]; then
	if [[ $1 -eq "update" ]]; then
		((build++))
		printf $build
	else
		build=$1
	fi
else
	printf $build
fi

cp $0 $0.tmp
awk -v build="$build" -- '/^build=/{print "build=" build;next}{print}' $0 > $0.tmp

{ sleep 0.25; mv $0.tmp $0; } &
exit

