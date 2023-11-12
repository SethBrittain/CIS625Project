#!/bin/bash
for i in {1..450}
do
	NUM1=$(($RANDOM % 20));
	NUM2=$(($RANDOM % 20));
	NUM3=$(($RANDOM % 20));
	echo "$NUM1 $NUM2 $NUM3";
done
