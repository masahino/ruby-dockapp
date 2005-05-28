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
#include "dockapp_utils.h"

static void dockpopup_show(VALUE self, VALUE x, VALUE y)
{
	WMDockApp *dock;
	WMDockItem *popup;
	int root_x, root_y;

	Data_Get_Struct(self, WMDockItem, popup);

	Check_Type(x, T_FIXNUM);
	Check_Type(y, T_FIXNUM);
	dock = popup->dock;
	
	if (popup->visible != DOCKITEM_VISIBLE) {
		XMapWindow(display, popup->win);
		XShapeCombineMask(display, popup->win,
				  ShapeBounding, 0, 0,
				  popup->xpm.mask, ShapeSet);
/*
	XSetClipMask(display, dock->NormalGC, popup->xpm.mask);
*/
		popup->visible = DOCKITEM_VISIBLE;

	}
	XCopyArea(display, popup->xpm.pixmap, popup->win,
		  dock->NormalGC,
		  0, 0, popup->width, popup->height, 0, 0);
	get_pointer_position(dock->win, &root_x, &root_y);
	XMoveWindow(display, popup->win, root_x - popup->width/2, root_y);
}

static void dockpopup_hide(VALUE self)
{
	WMDockApp *dock;
	WMDockItem *popup;
	Data_Get_Struct(self, WMDockItem, popup);

	dock = popup->dock;

	XUnmapWindow(dock->display, popup->win);
	XFlush(dock->display);
	popup->visible = DOCKITEM_INVISIBLE;
}

VALUE dockpopup_initialize(int argc, VALUE *argv, VALUE self)
{
	VALUE obj;
	WMDockItem *popup;
	VALUE width, height;
	XSetWindowAttributes att;

	if (rb_scan_args(argc, argv, "20", &width, &height) == 2) {
		;
	} else {
		;
	}
	Check_Type(width, T_FIXNUM);
	Check_Type(height, T_FIXNUM);
	  
	popup = malloc(sizeof(WMDockItem));
	memset(popup, 0, sizeof(WMDockItem));
	popup->width = FIX2INT(width);
	popup->height = FIX2INT(height);
	popup->type = TYPE_POPUP;

	popup->win = XCreateSimpleWindow(display, Root, 
					 0, 0,
					 popup->width, popup->height,
					 0, BlackPixel(display,0), 
					 WhitePixel(display,0) );
	att.override_redirect=True;
	XChangeWindowAttributes (display, popup->win,
				 CWOverrideRedirect, &att);
	obj = Data_Wrap_Struct(self, dockitem_mark, -1, popup);

	
	return obj;
}

VALUE dockpopupimage_initialize(VALUE self, VALUE xpm_file)
{
	VALUE obj;
	WMDockItem *popup;
	XSetWindowAttributes att;

	Check_Type(xpm_file, T_STRING);

	/* TODO: xpm_fileの存在確認して、無ければraise) */

	popup = malloc(sizeof(WMDockItem));
	memset(popup, 0, sizeof(WMDockItem));

	popup->type = TYPE_POPUP;

	GetXPMfromFile(&(popup->xpm), StringValuePtr(xpm_file));
	popup->width = popup->xpm.attributes.width;
	popup->height = popup->xpm.attributes.height;
	popup->win = XCreateSimpleWindow(display, Root, 
					 0, 0,
					 popup->width, popup->height,
					 0, BlackPixel(display,0), 
					 WhitePixel(display,0) );
    	printf ("width = %d\n", popup->width);
	printf ("height = %d\n", popup->height);
	att.override_redirect=True;
	XChangeWindowAttributes (display, popup->win,
				 CWOverrideRedirect, &att);
	
	obj = Data_Wrap_Struct(self, dockitem_mark, -1, popup);
	
	return obj;
}

void dockpopup_init(VALUE rb_DockApp)
{
	VALUE rb_DockPopUp;
	VALUE rb_DockPopUpImage;

	rb_DockPopUp = rb_define_class_under(rb_DockApp, "PopUp", rb_cObject);
	rb_define_singleton_method(rb_DockPopUp, "new",
				   dockpopup_initialize, -1);
	rb_define_method(rb_DockPopUp, "show",
			 RUBY_METHOD_FUNC(dockpopup_show), 2);
	rb_define_method(rb_DockPopUp, "hide",
			 RUBY_METHOD_FUNC(dockpopup_hide), 0);


	rb_DockPopUpImage = rb_define_class_under(rb_DockApp, "PopUpImage",
					     rb_cObject);
	rb_define_singleton_method(rb_DockPopUpImage, "new", 
				   dockpopupimage_initialize, 1);
	rb_define_method(rb_DockPopUpImage, "show",
			 RUBY_METHOD_FUNC(dockpopup_show), 2);
	rb_define_method(rb_DockPopUpImage, "hide",
			 RUBY_METHOD_FUNC(dockpopup_hide), 0);

}
