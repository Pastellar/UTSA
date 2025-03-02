#!/bin/bash

if [ ! -f "./project2" ]; then
  make -f Makefile
fi

if (( $# == 1 )); then
	./project2 $1
else
	printf "Invalid number of parameters.\nUsage: bash project2.sh <input.txt>\n"
fi
