# Frank Hale <frankhale@gmail.com>
#
# January 18, 2005
CC       = g++
CFLAGS   = -g -O2 -Wall

CODENAME = xclock

PREFIX   = /usr
DESTDIR  =
XROOT    =
INCLUDES = -I$/usr/X11R6/include
LDPATH   = -L/usr/X11R6/lib
LIBS     = -lXext -lX11 

DEFINES  = #-DDEBUG 

HEADERS  = xclock.hh
	   
OBJS     = xclock.o

all: $(CODENAME)

$(CODENAME): $(OBJS)
	$(CC) $(OBJS) $(LDPATH) $(LIBS) -o $@

$(OBJS): %.o: %.cc $(HEADERS)
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -c $< -o $@

install: all
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	install -s $(CODENAME) $(DESTDIR)$(PREFIX)/bin

clean:
	rm -f $(CODENAME) $(OBJS) core
	
