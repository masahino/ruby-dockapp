
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdarg.h>
#include <X11/Xlib.h>
#include <X11/xpm.h>
#include <X11/extensions/shape.h>

#include "ruby.h"

#include "dockapp.h"

int get_Xsignal_type(char *signal_type)
{
	if (strcmp(signal_type, "button_press_event") == 0) {
		return ButtonPress;
	} else if (strcmp(signal_type, "button_release_event") == 0) {
		return ButtonRelease;
	} else if (strcmp(signal_type, "selection_notify_event") == 0) {
		return SelectionNotify;
	}
	return -1;
}

int wait_Xevent(WMDockApp *dock, int event_type)
{
	XEvent	event;
	Display *d;

	d = dock->display;

	while (XPending(d)) {
		XNextEvent(d, &event);
		if (event.type == event_type) {
			return 0;
		}
		usleep(10000); /* 10ms */
	}
	return -1;
}

