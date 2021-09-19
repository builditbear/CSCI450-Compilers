CC = gcc
PROG = atl0
OBJS = main.o baltree.o parse.o scan.o util.o
CFLAGS = -g -Wall

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $(PROG) $(OBJS)

clean:
	rm -f $(OBJS) *~ atl.out atl.asm .atl.*

realclean: clean
	rm -f $(PROG)

main.o:    defs.h global.h
baltree.o: baltree.h
parse.o:   defs.h global.h scan.h baltree.h
scan.o:    defs.h global.h scan.h
util.o:    defs.h global.h

