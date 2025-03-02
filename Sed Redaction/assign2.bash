#!/bin/bash

#Set the file for todays date
sed -r "s/<date>/$(date '+%m-%d-%Y')/g" -i "$1"
#Run the .sed file to apply the changes
sed -r -f assign2.sed -i "$1"