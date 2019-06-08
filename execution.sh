#!/bin/bash

if [ -z $1 ]; then
	echo "For matrix generator use "g" key."
	echo "For matrix operation use "o" key."
	echo "For testing use "t" key."
elif [ $1 = "g" ]; then
	 ./matrix_generator.exe
elif [ $1 = "o" ]; then
	./task_matrix_combination.exe
elif [ $1 = "t" ]; then
	./testing.exe
else
	echo "Error: Incorrect command"
fi
