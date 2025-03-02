# Define the machine object files for your program
OBJECTS = Project1.o
# Define your include file
INCLUDES = Project1.h

# make for the executable
project1: ${OBJECTS}
	gcc -g -o project1run ${OBJECTS}

# Simple suffix rules for the .o
%.o: %.c ${INCLUDES}
	gcc -g -c $<

# Clean the .o files
clean:
	rm -f ${OBJECTS}
