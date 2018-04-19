#!/bin/bash

h_files=(paging.h proc.h)
sys_files=(initialize.c i386.c main.c create.c kill.c)
compile_files=(Makefile)
paging_files=(xm.c vgetmem.c vfreemem.c bsm.c release_bs.c get_bs.c vcreate.c policy.c frame.c pfint.c pfintr.S control_reg.c)

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

i="0"
while [ $i -lt ${#paging_files[@]} ]
do
	echo "cp ./paging/${paging_files[i]} ./TMP/"
	cp ./paging/${paging_files[i]} ./TMP/
	i=$[$i+1]
done


