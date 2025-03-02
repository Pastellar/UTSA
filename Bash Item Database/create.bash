#!/bin/bash

#Function creates the new item file and fills it with data
create_new_file(){
	file_name="./data/$item_number.item"
	echo "$simple_name $item_name" >> $file_name
	echo "$curr_quantity $max_quantity" >> $file_name
	echo "$item_description" >> $file_name

	echo "CREATED: $(date) - $simple_name" >> "./data/queries.log"
}

#read in the information line by line for the new file
read -p 'Item number: ' item_number
read -p 'Simple name: ' simple_name
read -p 'Item name: ' item_name
read -p 'Current quantity: ' curr_quantity
read -p 'Maximum quantity: ' max_quantity
read -e -p 'Description: ' item_description

#check if the file already exists
if [ -f "./data/$item_number.item" ] 
then
	echo "ERROR: item already exists"
else
	create_new_file
fi
