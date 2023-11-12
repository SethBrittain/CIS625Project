#! /bin/bash

mkdir outputs

make all > /dev/null

NUM_POINTS=$1
R_MAX=$2
MAX_COORD=$3

echo -e "Running tests with $NUM_POINTS points, rmax of $R_MAX and max coordinate $MAX_COORD:"

./gendata.sh $NUM_POINTS $MAX_COORD > inputfile.dat

echo -en "\nC++ time:"
time ./findTris inputfile.dat $R_MAX $NUM_POINTS | sort -V > outputs/cpp.output

echo -en "\nPython time:"
time python3 findTris.py inputfile.dat $R_MAX $NUM_POINTS | sort -V > outputs/python.output

echo -e "\nResults:"
if $(diff outputs/cpp.output outputs/python.output);
then
	echo -en "Outputs match!\n"
else
	echo -en "Outputs differ!\n"
fi