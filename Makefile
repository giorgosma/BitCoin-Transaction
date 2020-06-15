#
# In order to execute this "Makefile" just type "make"
#	A. Delis (ad@di.uoa.gr)
#

OBJS 	= bitcoin.o structures.o functions.o
SOURCE	= bitcoin.c structures.c functions.c
HEADER  = functions.h structures.h
OUT  	= bitcoin
CC	= gcc
FLAGS   = -g -c 
# -g option enables debugging mode 
# -c flag generates object code for separate files

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT)

# create/compile the individual files >>separately<< 
bitcoin.o: bitcoin.c
	$(CC) $(FLAGS) bitcoin.c

structures.o: structures.c
	$(CC) $(FLAGS) structures.c

functions.o: functions.c
	$(CC) $(FLAGS) functions.c

# clean house
clean:
	rm -f $(OBJS) $(OUT)

# do a bit of accounting
count:
	wc $(SOURCE) $(HEADER)
