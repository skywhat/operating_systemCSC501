#!/bin/bash

h_files=()
sys_files=()
compile_files=()

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

i="0"
while [ $i -lt ${#compile_files[@]} ]
do
	echo "cp ./compile/${compile_files[i]} ./TMP/"
	cp ./compile/${compile_files[i]} ./TMP/
	i=$[$i+1]
done
