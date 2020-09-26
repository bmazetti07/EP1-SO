C	= gcc
CFLAGS	= -g -Wall
RLINE	= -lreadline
RTHREAD = -lpthread

#---------------------------------------------

all: bccsh ep1

bccsh: bccsh.c
	$(C) $(CFLAGS) -o bccsh bccsh.c $(RLINE)

ep1: ep1.c
	$(C) $(CFLAGS) -o ep1 ep1.c $(RTHREAD)

clean:
	$(RM) *~ *.o
