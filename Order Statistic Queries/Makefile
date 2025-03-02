# Define the machine object files for your program
OBJECTS = Project2.o Store.o
# Define your include file
INCLUDES = Store.h

# make for the executable
project1: ${OBJECTS}
	gcc -g -o project2 ${OBJECTS} -lm

# Simple suffix rules for the .o
%.o: %.c ${INCLUDES}
	gcc -g -c $<

# Clean the .o files
clean:
	rm -f ${OBJECTS}
