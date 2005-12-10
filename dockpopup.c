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

#define DOCKPOPUP_DOWN 0
#define DOCKPOPUP_UP 1

static int get_menu_index(WMDockItem *popup)
{
	WMDockApp *dock;
	int root_x, root_y;
	
	dock = popup->dock;
	get_pointer_position(dock->win, &root_x, &root_y);

#ifdef DEBUG
	printf ("x = %d, y = %d\n", root_x, root_y);
	printf ("px = %d, py = %d\n", popup->x, popup->y);
	printf ("w = %d, h = %d\n", popup->width, popup->height);
#endif
	if (root_x < popup->x || root_x > popup->x + popup->width ||
	    root_y < popup->y || root_y > popup->y + popup->height) {
		return Qnil;
	}
#ifdef DEBUG
	printf ("index = %d\n", (root_y-popup->y)/(LEDCHAR_HEIGHT+1));
#endif
	return INT2FIX((root_y-popup->y)/(LEDCHAR_HEIGHT+1));
}

static void dockpopup_hide_menu(WMDockItem *popup)
{
	WMDockApp *dock;
	dock = popup->dock;

	XUnmapWindow(dock->display, popup->win);
	XFlush(dock->display);
	popup->visible = DOCKITEM_INVISIBLE;
}

static void dockpopup_show_menu(WMDockItem *popup, int direction)
{
	WMDockApp *dock;
	int root_x, root_y;
	
	dock = popup->dock;

	if (popup->visible != DOCKITEM_VISIBLE) {
		XMapWindow(display, popup->win);
		XShapeCombineMask(display, popup->win,
				  ShapeBounding, 0, 0,
				  popup->xpm.mask, ShapeSet);
		popup->visible = DOCKITEM_VISIBLE;

	}
	get_pointer_position(dock->win, &root_x, &root_y);
	popup->x = root_x - popup->width/2;
	popup->y = root_y;
	if (direction == DOCKPOPUP_UP) {
		popup->y = root_y - popup->height;
	}
	XMoveWindow(display, popup->win, popup->x, popup->y);
	RedrawWindow2(display, popup->xpm.pixmap, popup->win,
		      dock->NormalGC, popup->width, popup->height);
}

static void dockpopup_popup(int argc, VALUE *argv, VALUE self)
{
	WMDockItem *popup;
	VALUE vdirection;
	int direction;
	int index;

	if (rb_scan_args(argc, argv, "01", &vdirection) == 0) {
		direction = DOCKPOPUP_DOWN;
	} else {
		Check_Type(vdirection, T_FIXNUM);
		direction = FIX2INT(vdirection);
	}

	Data_Get_Struct(self, WMDockItem, popup);

	dockitem_hide_tooltips(popup->dock);
	dockpopup_show_menu(popup, direction);
	/* event loop */
	do { /* dangerous */
		if (wait_Xevent(popup->dock, ButtonRelease) != -1) {
			break;
		}
	} while (1);
	index = get_menu_index(popup);
	dockpopup_hide_menu(popup);
	
	rb_funcall(rb_block_proc(), id_call, 1, index);
}

static VALUE dockpopup_get_index(VALUE self)
{
	WMDockItem *popup;
	WMDockApp *dock;
	int root_x, root_y;

	Data_Get_Struct(self, WMDockItem, popup);
	dock = popup->dock;
	get_pointer_position(dock->win, &root_x, &root_y);

#ifdef DEBUG
	printf ("x = %d, y = %d\n", root_x, root_y);
	printf ("px = %d, py = %d\n", popup->x, popup->y);
	printf ("w = %d, h = %d\n", popup->width, popup->height);
#endif
	if (root_x < popup->x || root_x > popup->x + popup->width ||
	    root_y < popup->y || root_y > popup->y + popup->height) {
		return Qnil;
	}
#ifdef DEBUG
	printf ("index = %d\n", (root_y-popup->y)/(LEDCHAR_HEIGHT+1));
#endif
	return INT2FIX((root_y-popup->y)/(LEDCHAR_HEIGHT+1));
}

static void make_menu_image(WMDockItem *popup)
{
	WMDockApp *dock;
	int dest_x = 1;
	int dest_y = 1;
	int color = 0;
	int max_width;
	int row = 0;
	int i;
	char **lines;

	lines = strsplit(popup->text, "\n", 0);
	
	max_width = 0;
	for (i = 0; lines[i] != NULL; i++) {
#ifdef DEBUG
		printf ("lines[%d] = \"%s\"\n", i, lines[i]);
#endif
		if (lines[i][0] == '\0') {
			break;
		}
		if (max_width < strlen(lines[i])) {
			max_width = strlen(lines[i]);
		}
	}
	if (i == 0) {
		return;
	}
	row = i;
	dock = popup->dock;
	popup->width = max_width*LEDCHAR_WIDTH + 3;
	popup->height = row*(LEDCHAR_HEIGHT+1) + 3;
	XResizeWindow(dock->display, popup->win, 
		      popup->width, popup->height);

	if (popup->xpm_master != NULL) {
		free(popup->xpm_master);
	}
	popup->xpm_master = init_pixmap_with_size(popup->width, popup->height);
	GetXPM2(&(popup->xpm), popup->xpm_master);
	mask_window2(popup->win, popup->xpm_master, popup->width, popup->height);

	draw_rect2(dock, popup->xpm, 0, 0, popup->width, popup->height, "#208120812081");
//"#2081B2CAAEBA");
	draw_rect2(dock, popup->xpm, 1, 1, popup->width-2, popup->height-2, "black");
	for (i = 0; i < row; i++) {
		drawnLEDString2(dock, popup->xpm, dest_x + 1, 
				dest_y + i * (LEDCHAR_HEIGHT+1) + 1, lines[i], 
				max_width, 
				color);
	}
	if (lines) {
		free(lines);
	}

}

static void dockpopup_add_item2(VALUE self, VALUE text)
{
	WMDockItem *popup;
	VALUE tmp;
	int i, n;
	char *str;

	str = malloc(sizeof(char*)*MAX_LIST_LINE);
	memset(str, 0, MAX_LIST_LINE);

	Check_Type(text, T_ARRAY);

	n = RARRAY(text)->len;
#ifdef DEBUG
	printf ("array size = %d\n", n);
#endif
	for (i = 0; i < n; i++) {
		tmp = rb_ary_shift(text);
		Check_Type(tmp, T_STRING);
#ifdef DEBUG
		printf ("%s\n", StringValuePtr(tmp));
#endif
		strcat(str, StringValuePtr(tmp));
		strcat(str, "\n");
	}
	Data_Get_Struct(self, WMDockItem, popup);

	
	if (popup->text == NULL || 
	    strcmp(popup->text, str) != 0) {
		if (popup->text != NULL) {
			free(popup->text);
		}
		popup->text = strdup(str);
		make_menu_image(popup);
	}

}

static void dockpopup_add_item(VALUE self, VALUE text)
{
	WMDockItem *popup;

	Check_Type(text, T_STRING);
	Data_Get_Struct(self, WMDockItem, popup);

	
	if (popup->text == NULL || 
	    strcmp(popup->text, StringValuePtr(text)) != 0) {
		if (popup->text != NULL) {
			free(popup->text);
		}
		popup->text = strdup(StringValuePtr(text));
		make_menu_image(popup);
	}
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

static void dockpopup_show(int argc, VALUE *argv, VALUE self)
{
	WMDockItem *popup;
	VALUE x, y, vdirection;
	int direction;

	if (rb_scan_args(argc, argv, "21", &x, &y, &vdirection) == 2) {
		direction = DOCKPOPUP_DOWN;
	} else {
		Check_Type(vdirection, T_FIXNUM);
		direction = FIX2INT(vdirection);
	}

	Data_Get_Struct(self, WMDockItem, popup);

	Check_Type(x, T_FIXNUM);
	Check_Type(y, T_FIXNUM);

	dockpopup_show_menu(popup, direction);
}

static void dockpopup_hide(VALUE self)
{
	WMDockItem *popup;
	Data_Get_Struct(self, WMDockItem, popup);

	dockpopup_hide_menu(popup);
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

	if (display == NULL) {
		rb_raise(rb_eRuntimeError, "not openwindow");
		return Qnil;
	}
	  
	popup = malloc(sizeof(WMDockItem));
	memset(popup, 0, sizeof(WMDockItem));
	popup->width = FIX2INT(width);
	popup->height = FIX2INT(height);
	popup->type = TYPE_POPUP;

	popup->win = XCreateSimpleWindow(display, Root, 
					 0, 0,
					 popup->width, popup->height,
					 0, BlackPixel(display, 0), 
					 WhitePixel(display, 0));
	att.override_redirect=True;
	XChangeWindowAttributes (display, popup->win,
				 CWOverrideRedirect, &att);

	popup->xpm_master = init_pixmap_with_size(popup->width, popup->height);
	GetXPM2(&(popup->xpm), popup->xpm_master);
#ifdef DEBUG
	printf ("width=%d,height=%d\n", popup->width, popup->height);
#endif
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
#ifdef DEBUG
		printf ("len = %d\n", len);
#endif
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
#ifdef DEBUG
    	printf ("width = %d\n", popup->width);
	printf ("height = %d\n", popup->height);
#endif 
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
	VALUE rb_DockPopUpText;

	rb_DockPopUp = rb_define_class_under(rb_DockApp, "PopUp", rb_cObject);
	rb_define_singleton_method(rb_DockPopUp, "new",
				   dockpopup_initialize, -1);
	rb_define_method(rb_DockPopUp, "add_item", 
			 RUBY_METHOD_FUNC(dockpopup_add_item2), 1);
	rb_define_method(rb_DockPopUp, "show",
			 RUBY_METHOD_FUNC(dockpopup_show), -1);
	rb_define_method(rb_DockPopUp, "hide",
			 RUBY_METHOD_FUNC(dockpopup_hide), 0);
	rb_define_method(rb_DockPopUp, "popup",
			 RUBY_METHOD_FUNC(dockpopup_popup), -1);
	rb_define_method(rb_DockPopUp, "get_index",
			 RUBY_METHOD_FUNC(dockpopup_get_index), 0);


	rb_define_const(rb_DockPopUp, "DOWN", INT2FIX(DOCKPOPUP_DOWN));
	rb_define_const(rb_DockPopUp, "UP", INT2FIX(DOCKPOPUP_UP));


	/* PopUpText */
	rb_DockPopUpText = rb_define_class_under(rb_DockApp, "PopUpText", rb_cObject);
	rb_define_singleton_method(rb_DockPopUpText, "new",
				   dockpopup_initialize, -1);
	rb_define_method(rb_DockPopUpText, "add_item", 
			 RUBY_METHOD_FUNC(dockpopup_add_item), 1);
	rb_define_method(rb_DockPopUpText, "show",
			 RUBY_METHOD_FUNC(dockpopup_show), -1);
	rb_define_method(rb_DockPopUpText, "hide",
			 RUBY_METHOD_FUNC(dockpopup_hide), 0);
	rb_define_method(rb_DockPopUpText, "popup",
			 RUBY_METHOD_FUNC(dockpopup_popup), -1);
	rb_define_method(rb_DockPopUpText, "get_index",
			 RUBY_METHOD_FUNC(dockpopup_get_index), 0);


	rb_define_const(rb_DockPopUpText, "DOWN", INT2FIX(DOCKPOPUP_DOWN));
	rb_define_const(rb_DockPopUpText, "UP", INT2FIX(DOCKPOPUP_UP));

	/* PopUpImage */
	rb_DockPopUpImage = rb_define_class_under(rb_DockApp, "PopUpImage",
					     rb_cObject);
	rb_define_singleton_method(rb_DockPopUpImage, "new", 
				   dockpopupimage_initialize, 1);
	rb_define_method(rb_DockPopUpImage, "show",
			 RUBY_METHOD_FUNC(dockpopup_show), -1);
	rb_define_method(rb_DockPopUpImage, "hide",
			 RUBY_METHOD_FUNC(dockpopup_hide), 0);

	rb_define_method(rb_DockPopUpImage, "width",
			 RUBY_METHOD_FUNC(dockpopup_width), 0);
	rb_define_method(rb_DockPopUpImage, "height",
			 RUBY_METHOD_FUNC(dockpopup_height), 0);

	
}
