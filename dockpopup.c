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

static void make_menu_image(WMDockItem *popup)
{
	WMDockApp *dock;
	int dest_x = 1;
	int dest_y = 1;
	int column = 0;
	int color = 0;
	int max_width;
	int row = 0;
	int i;
	char *str_ptr;
	char **lines;

	lines = strsplit(popup->text, "\n", 0);
	
	max_width = 0;
	for (i = 0; lines[i] != NULL; i++) {
		if (max_width < strlen(lines[i])) {
			max_width = strlen(lines[i]);
		}
	}
	if (i == 0) {
		return;
	}
	row = i-1;
	dock = popup->dock;
	popup->width = max_width*LEDCHAR_WIDTH + 1;
	popup->height = row*(LEDCHAR_HEIGHT+1);
	XResizeWindow(dock->display, Root, 
		      popup->width, popup->height);

	popup->xpm_master = init_pixmap_with_size(popup->width, popup->height);
	GetXPM2(&(popup->xpm), popup->xpm_master);
	mask_window2(popup->win, popup->xpm_master, popup->width, popup->height);

	for (i = 0; lines[i] != NULL; i++) {
		drawnLEDString2(dock, popup->xpm, dest_x, 
				dest_y + i * LEDCHAR_HEIGHT, lines[i], 
				strlen(lines[i]),
				color);
	}

}

static void dockpopup_add_item(VALUE self, VALUE text)
{
	WMDockItem *popup;

	Check_Type(text, T_STRING);
	Data_Get_Struct(self, WMDockItem, popup);

	popup->text = strdup(StringValuePtr(text));
	make_menu_image(popup);
}

static VALUE dockpopup_width(VALUE self)
{
	WMDockItem *popup;

	Data_Get_Struct(self, WMDockItem, popup);
	return INT2FIX(popup->width);
}

static VALUE dockpopup_height(VALUE self)
{
	WMDockItem *popup;

	Data_Get_Struct(self, WMDockItem, popup);
	return INT2FIX(popup->height);
}

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
/*
	XCopyArea(display, popup->xpm.pixmap, popup->win,
		  dock->NormalGC,
		  0, 0, popup->width, popup->height, 0, 0);
*/
	get_pointer_position(dock->win, &root_x, &root_y);
	XMoveWindow(display, popup->win, root_x - popup->width/2, root_y);
	RedrawWindow2(display, popup->xpm.pixmap, popup->win,
		      dock->NormalGC, popup->width, popup->height);
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

	popup->xpm_master = init_pixmap_with_size(popup->width, popup->height);
	GetXPM2(&(popup->xpm), popup->xpm_master);
	printf ("width=%d,height=%d\n", popup->width, popup->height);
	mask_window2(popup->win, popup->xpm_master, popup->width, popup->height);

	obj = Data_Wrap_Struct(self, dockitem_mark, -1, popup);

	
	return obj;
}

VALUE dockpopupimage_initialize(VALUE self, VALUE xpm_data)
{
	VALUE obj;
	WMDockItem *popup;
	XSetWindowAttributes att;

	popup = malloc(sizeof(WMDockItem));
	memset(popup, 0, sizeof(WMDockItem));
	popup->type = TYPE_POPUP;

	if (TYPE(xpm_data) == T_STRING) {
		/* TODO: xpm_fileの存在確認して、無ければraise) */
		GetXPMfromFile(&(popup->xpm), StringValuePtr(xpm_data));
	} else if (TYPE(xpm_data) == T_ARRAY) {
		char **data;
		int len;
		int i;
		len = RARRAY(xpm_data)->len;
		printf ("len = %d\n", len);
		data = malloc(sizeof(char*)*len);
		for (i = 0; i < len; i++) {
			data[i] = strdup(StringValuePtr(RARRAY(xpm_data)->ptr[i]));
		}
		GetXPMfromData(&(popup->xpm), data);
	} else {
		exit(0);
	}
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
	rb_define_method(rb_DockPopUp, "add_item", 
			 RUBY_METHOD_FUNC(dockpopup_add_item), 1);
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

	rb_define_method(rb_DockPopUpImage, "width",
			 RUBY_METHOD_FUNC(dockpopup_width), 0);
	rb_define_method(rb_DockPopUpImage, "height",
			 RUBY_METHOD_FUNC(dockpopup_height), 0);
}
