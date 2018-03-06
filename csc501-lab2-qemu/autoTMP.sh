#!/bin/bash

h_files=(lock.h proc.h q.h)
sys_files=(linit.c lcreate.c ldelete.c lock.c releaseall.c initialize.c resched.c getprio.c create.c kill.c getprio.c chprio.c)


i="0"
while [ $i -lt ${#h_files[@]} ]
do
	echo "cp ./h/${h_files[i]} ./TMP/"
	cp ./h/${h_files[i]} ./TMP/
	i=$[$i+1]
done


i="0"
while [ $i -lt ${#sys_files[@]} ]
do
	echo "cp ./sys/${sys_files[i]} ./TMP/"
	cp ./sys/${sys_files[i]} ./TMP/
	i=$[$i+1]
done
