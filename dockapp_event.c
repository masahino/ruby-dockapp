/*
  $Id$
  
  Copyright (c) 2005 HINO Masayuki <masahino@tky3.3web.ne.jp>
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdarg.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/xpm.h>
#include <X11/extensions/shape.h>

#include "ruby.h"
#include "dockapp.h"

VALUE rb_DockEvent;

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

/*
 * call-seq:
 *   button
 *
 */
VALUE dockevent_button(VALUE self)
{
	struct WMDockEvent *event;
	Data_Get_Struct(self, struct WMDockEvent, event);
	return INT2FIX(event->event.xbutton.button);
}

/*
 * call-seq:
 *   x
 *
 */
VALUE dockevent_x(VALUE self)
{
	struct WMDockEvent *event;
	Data_Get_Struct(self, struct WMDockEvent, event);
	return INT2FIX(event->event.xbutton.x);
}

/* 
 * call-seq:
 *   y
 *
 */
VALUE dockevent_y(VALUE self)
{
	struct WMDockEvent *event;
	Data_Get_Struct(self, struct WMDockEvent, event);
	return INT2FIX(event->event.xbutton.y);
}

/*
 * call-seq:
 *   new(xevent)
 *
 */
VALUE dockevent_initialize(VALUE self, XEvent xevent)
{
	VALUE obj;
	struct WMDockEvent *dockevent;

	dockevent = malloc(sizeof(struct WMDockEvent));
	dockevent->event = xevent;

	obj = Data_Wrap_Struct(rb_DockEvent, NULL, -1, dockevent);
	
	return obj;
}

void dockevent_init(VALUE rb_DockApp)
{

#if 0 /* RDoc */
     rb_DockApp = rb_define_class("DockApp", rb_cObject);
#endif
	rb_DockEvent = rb_define_class_under(rb_DockApp, "Event", rb_cObject);
	rb_define_singleton_method(rb_DockEvent, "new", dockevent_initialize, 1);
	rb_define_method(rb_DockEvent, "button",
			 RUBY_METHOD_FUNC(dockevent_button), 0);
	
	rb_define_method(rb_DockEvent, "x",
			 RUBY_METHOD_FUNC(dockevent_x), 0);
	rb_define_method(rb_DockEvent, "y",
			 RUBY_METHOD_FUNC(dockevent_y), 0);
	
}
