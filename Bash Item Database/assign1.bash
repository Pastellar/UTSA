#!/bin/bash

menu() {
echo "Enter one of the following actions or press CTRL-D to exit.
      C - create new item
      R - read an existing item
      U - update an existing item
      D - delete an existing item"
}

#if user enters something outside of the menu options
incorrect_selection() {
        echo "ERROR: invalid option"
}

press_enter() {
        echo -n ""
        read
        clear; 
}

#check if a data directory exists, create one if not
if [ ! -d "./data" ]; then
        mkdir data
fi  

#display the menu and read in user options
while true; do
	clear
        menu
        if ! read action; then break; fi
        case "$action" in
        [Cc]) clear ; ./create.bash ; press_enter ;;
        [Rr]) clear ; ./read.bash ; press_enter ;;
        [Uu]) clear ; ./update.bash ; press_enter ;;
        [Dd]) clear ; ./delete.bash ; press_enter ;;
        * ) clear ; incorrect_selection ; press_enter ;;
        esac
done


