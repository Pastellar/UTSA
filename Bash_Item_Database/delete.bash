#!/bin/bash

read -p 'Enter an item number: ' item_number

#check if the file exists before removing
if [ -f "./data/$item_number.item" ]; then
	#remove the file
	rm "./data/$item_number.item"
	
	#update the log
	echo "DELETED: $(date) - $item_number" >> "./data/queries.log"

	else
	echo "ERROR: item not found"
	exit
fi
