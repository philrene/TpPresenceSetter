PROGS     = setPresence
CXX	= gcc
CSRCS     = main.c gui.c setpresence.c

OBJS      = $(CSRCS:.c=.o)

CFLAGS   += -Wall -g

LIBS	= `pkg-config --libs --cflags gtk+-2.0 telepathy-glib`
##########################################################################

all:	$(PROGS)

install:	all

$(PROGS):	
	$(CXX) $(CFLAGS) $(CSRCS) -o $@ $(LIBS)
clean:
	rm -f $(PROGS) $(OBJS) *~
