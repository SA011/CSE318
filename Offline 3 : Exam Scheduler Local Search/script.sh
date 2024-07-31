#!/bin/bash
echo "====EXPONENTIAL PENALTY====" > out.txt
for ((i = 0; i < 4; i++))
do
	for ((j = 0; j < 5; j++))
	do
		./1805106 $i 1 $j 20005 >> out.txt
	done
done
echo "" >> out.txt
echo "" >> out.txt
echo "====LINEAR PENALTY====" >> out.txt
for ((j = 0; j < 5; j++))
do
	./1805106 1 0 $j 20005 >> out.txt
done
