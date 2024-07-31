#!/bin/bash

for i in 1 2 3 4 5 6
do
	echo "Testing with Dataest#$i"
	for j in 0 1 2 3 4
	do
		echo ""
		echo "VAH$((j + 1))"
		echo "=========="
		echo "BACKTRACK Testing"
		echo "=========="
		echo ""
		time ./a 0 $j < test$i.txt
		echo "==========="
		echo ""
	  echo "=========="
		echo "Forward Check Testing"
		echo "=========="
		echo ""
		time ./a 1 $j < test$i.txt
		echo "==========="
		echo ""
		echo ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"
	done 
	
	done 
done
