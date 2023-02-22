#!/bin/bash

#Reads the file
file_contents() {
	read -p simple_name item_name
	read -p curr_quantity max_quantity
	read -p -e item_description
}


#read the desired file
read -p 'Item number: ' item_number
read -p 'Simple name: ' simple_name
read -p 'Item name: ' item_name
read -p 'Current quantity: ' curr_quantity
read -p 'Maximum quantity: ' max_quantity
read -p 'Description: '  item_description


if [ -f "data/$item_number.item" ]; then
	#read the stored variables and update according to the user's new inputed data
	while true; do
		read old_simple_name old_item_name
		read old_curr_qty old_max_qty
		read old_description
		break
	done < "data/$item_number.item"

	if [ -z $simple_name ]; 
	then
		simple_name=$old_simple_name
	fi
	
	if [ -z $item_name ]; 
	then
		item_name=$old_item_name
	fi

	if [ -z $curr_quantity ];
	then
		curr_quantity=$old_curr_qty
	fi

	if [ -z $max_quantity ];
	then
		max_quantity=$old_max_qty
	fi

	if [ -z $item_description ];
	then
		item_description=$old_description
	fi
	
	#write the new data to file
	echo "$simple_name $item_name" > "./data/$item_number.item"
	echo "$curr_quantity $max_quantity" >> "./data/$item_number.item"
	echo "$item_description" >> "./data/$item_number.item"

	#update the log
	echo "UPDATED: $(date) - $simple_name" >> "./data/queries.log"

	else 
		echo "ERROR: item not found"
fi