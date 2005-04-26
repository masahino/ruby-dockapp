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



VALUE dockevent_initialize(VALUE self, VALUE xevent)
{
	VALUE obj;

/*	obj = Data_Wrap_Struct(self, dockitem_mark, -1, popup);*/
	
	return obj;
}

void dockevent_init(VALUE rb_DockApp)
{
	VALUE rb_DockEvent;

	rb_DockEvent = rb_define_class_under(rb_DockApp, "Event", rb_cObject);
	rb_define_singleton_method(rb_DockEvent, "new", dockevent_initialize, 1);

}
