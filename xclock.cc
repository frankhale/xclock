// xclock - a small clock for X11
// 
// Contact Info:
//
//  Frank Hale - <frankhale@gmail.com> 
//               <http://about.me/frank.hale>
//
// Date: January 24, 2005
//
// GPL version 3 <http://www.gnu.org/licenses/gpl-3.0.html>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software F:oundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include "xclock.hh"

class XClock {

private:
	Display *dpy;
	Window win, root;
	XEvent event;
	int screen;
	
	fd_set fd;
	struct timeval tv;
	int xfd;
	time_t now;
	struct tm *lt;
	
public:
	XClock(); 

	void drawClock();

	void eventHandler();

	void quit();
	static void signal_handler(int signal);
};

XClock *xclock;

XClock::XClock()
{
	XSetWindowAttributes sattr;
	
	xclock = this;
	
	dpy = XOpenDisplay(NULL);
	
	if(!dpy)
	{
		cerr << "Can't open display" << endl;
		exit(0);
	}
	
	root = RootWindow(dpy,0);
	
	signal(SIGTERM, signal_handler);
	signal(SIGINT, signal_handler);
	
	screen=DefaultScreen(dpy);
	
	win = XCreateSimpleWindow(dpy, root, 5, 5, 130, 20, 1, BlackPixel(dpy, screen), WhitePixel(dpy, screen));

	sattr.override_redirect=True;
	
	XChangeWindowAttributes(dpy, win, CWOverrideRedirect, &sattr);
	
	XMapWindow(dpy, win);

	XSelectInput (dpy, win, ExposureMask);

	xfd = ConnectionNumber (dpy);

	XSync (dpy, False);

	drawClock();
}

void XClock::eventHandler()
{
	while(1)
	{
		now = time (0);
		lt = gmtime (&now);
		tv.tv_usec = 0;
		tv.tv_sec = 1;
		FD_ZERO (&fd);
		FD_SET (xfd, &fd);
		if (select (xfd + 1, &fd, 0, 0, &tv) == 0)
			 drawClock();
		
		while(XPending(dpy))
		{
			XNextEvent(dpy, &event);
  		
			switch  (event.type) 
			{
				case Expose:
					drawClock();
				break;
   			}
		}
	}
}

void XClock::quit()
{
	XDestroyWindow(dpy, win);
	XCloseDisplay(dpy);
	exit(0);
}

void XClock::signal_handler(int signal)
{
	switch(signal) 
  	{ 
		case SIGINT: 
  		case SIGTERM: 
			  xclock->quit();
    		break;
  	} 
}

void XClock::drawClock()
{
	char *time_str;
	time_t now;
	now = time (0);
	time_str = ctime (&now) + 4;

	XClearWindow(dpy, win);
	XDrawString(dpy, win, DefaultGC(dpy, screen), 6, 15, time_str, strlen(time_str)-1);
}

int main()
{
	XClock xc;

  xc.eventHandler();

	return 0;
}
