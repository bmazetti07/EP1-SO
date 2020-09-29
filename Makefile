C	= gcc
CFLAGS	= -g -Wall
RLINE	= -lreadline
RTHREAD = -lpthread

#---------------------------------------------

all: bccsh ep1

bccsh: bccsh.c
	$(C) $(CFLAGS) -o bccsh bccsh.c $(RLINE)

ep1: ep1.c header.o
	$(C) -o ep1 ep1.c header.o $(RTHREAD)

header.o: header.c	
	$(C) $(CFLAGS) -c header.c

clean:
	$(RM) *~ *.o
