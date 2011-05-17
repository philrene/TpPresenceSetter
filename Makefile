PROGS     = presence	 
CXX	= gcc
CSRCS     = main.c 

OBJS      = $(CSRCS:.c=.o)

CFLAGS   += -Wall -g

LIBS	= `pkg-config --libs --cflags telepathy-glib`
##########################################################################

all:	$(PROGS)

install:	all

$(PROGS):	
	$(CXX) $(CFLAGS) $(CSRCS) -o $@ $(LIBS)
clean:
	rm -f $(PROGS) $(OBJS) *~
