C	= gcc
CFLAGS	= -g -Wall
RLINE	= -lreadline

#---------------------------------------------

bccsh: bccsh.c
	$(C) $(CFLAGS) -o bccsh bccsh.c $(RLINE)

clean:
	$(RM) *~ *.o
