#!/bin/bash

NUM_POINTS=$1
MAX_COORD=$2

for (( i=0 ; i<$NUM_POINTS ; i++ ))
do
	NUM1=$(($RANDOM % $MAX_COORD));
	NUM2=$(($RANDOM % $MAX_COORD));
	NUM3=$(($RANDOM % $MAX_COORD));
	echo -e "$NUM1 $NUM2 $NUM3";
done

