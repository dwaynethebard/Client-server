#####################################
#Name:Darren Ytsma					#
#student number:301171125			#
#SFU username:dytsma				#
#Lecture section: CPMT 300 D100		#
#instructor: Brian Booth			#
#TA: Scott Kristjanson				#
#####################################

.SUFFIXES: .h .o .c

CCMAIN = client.c memwatch.c server.c
CC = gcc
CCOPTS = -g
LIBS = -lm
OBJS1 = client.o decrypt.o memwatch.o FUNctions.o socketfunctions.o
OBJS2 = server.o memwatch.o FUNctions.o socketfunctions.o
CCEXEC1 = lyrebird.client
CCEXEC2 = lyrebird.server


all:	$(CCEXEC1) $(CCEXEC2)

$(CCEXEC1):	$(OBJS1) makefile 
	@echo Linking $@ . . .
	$(CC) $(CCOPTS) $(OBJS1) -o $@ $(LIBS)

$(CCEXEC2):	$(OBJS2) makefile 
	@echo Linking $@ . . .
	$(CC) $(CCOPTS) $(OBJS2) -o $@ $(LIBS)

%.o:	%.c
	@echo Compiling $< . . .
	$(CC) -DMEMWATCH -DMW_STDIO -c $(CCOPTS) $<

clean:
	rm -f $(OBJS1)
	rm -f $(OBJS2)
	rm -f $(CCEXEC1)
	rm -f $(CCEXEC2)
	rm -f core
	rm -f memwatch.log



# references
# http://stackoverflow.com/questions/19230621/cant-catch-apostrophe-in-if-statement   showed me how to check for ' \' ' and '/''
# http://www.tutorialspoint.com   for how the fprintf(), fgets(),ctime() ect are used.
# http://man7.org/linux/man-pages/man2/getpid.2.html  
# https://www.google.ca/search?q=fork+c+child+knows+its+pid&ie=UTF-8&sa=Search&channel=fe&client=browser-ubuntu&hl=en&gws_rd=cr,ssl&ei=za0WVv_jLM6qogShqKTwCA#channel=fe&hl=en&q=fork+c
#http://stackoverflow.com/questions/11322488/how-to-make-sure-that-waitpid-1-stat-wnohang-collect-all-children-process
#
#
#
