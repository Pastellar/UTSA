#!/bin/bash

#prompt user to enter the item number
echo "Enter an item number:" 
read item_number
file_name="data/$item_number.item"

#check if the item exists
if [ -e data/$item_number".item" ];then
while true; do
	read simple_name item_name
	read curr_quantity max_quantity
	read item_description
	break
done < $file_name

echo "Item name: $item_name"
echo "Simple name: $simple_name"
echo "Item number: $item_number"
echo "Qty: $curr_quantity/$max_quantity"
echo "Description: $item_description"

else	
	echo "ERROR: item not found"
fi