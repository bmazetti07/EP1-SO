C	= gcc
CFLAGS	= -Wall -g

#---------------------------------------------

bccsh: bccsh.c
	$(C) $(CFLAGS) -o bccsh bccsh.c

clean:
	$(RM) *~ *.o