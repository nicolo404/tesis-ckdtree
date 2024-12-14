SRCDIR = ./src

CC          = cc 
CFLAGS      =  -O9 -lm 

all:  mytest  cleanO

clean: 
	rm -f *.o core exp1 

cleanO: 
	rm -f *.o   

mytest:   basics.o bitrankw32int.o dacs.o
	$(CC) $(CFLAGS) basics.o bitrankw32int.o dacs.o -o exp1 $(SRCDIR)/exp1.c -lm 
	
dacs.o:  basics.o bitrankw32int.o
	$(CC) $(CFLAGS) -c $(SRCDIR)/dacs.c

bitrankw32int.o: 
	$(CC) $(CFLAGS) -c $(SRCDIR)/$(SRCDIRUTILS)/bitrankw32int.c
	
basics.o: basics.c
	$(CC) $(CFLAGS) -c $(SRCDIR)/$(SRCDIRUTILS)/basics.c
	
basics.c: basics.h
#	touch $(SRCDIR)/$(SRCDIRUTILS)/basics.c	

basics.h: 
#	touch $(SRCDIR)/$(SRCDIRUTILS)/basics.h
	
