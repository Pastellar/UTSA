#!/bin/bash

if [ ! -f "./project1run" ]; then
  make -f Makefile
fi

if (( $# == 1 )); then
	./project1run $1
else
	echo "Inocrrect number of parameters.\nUsage: bash project1.sh <input.txt>"
fi