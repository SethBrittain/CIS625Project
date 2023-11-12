#!/bin/bash

NUM_POINTS=$1
MAX_COORD=$2

for (( i=0 ; i<$NUM_POINTS ; i++ ))
do
	NUM1=$(($RANDOM % $MAX_COORD));
	echo -n "$NUM1 ";
done
echo ""
for (( i=0 ; i<$NUM_POINTS ; i++ ))
do
	NUM1=$(($RANDOM % $MAX_COORD));
	echo -n "$NUM1 ";
done
echo ""
for (( i=0 ; i<$NUM_POINTS ; i++ ))
do
	NUM1=$(($RANDOM % $MAX_COORD));
	echo -n "$NUM1 ";
done

