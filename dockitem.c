/*
  $Id$
*/

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

static void dockitem_signal_connect(VALUE self, VALUE signal_type)
{

	WMDockItem *item;
	struct WMDockSignal *signal, *tmp;

	Data_Get_Struct(self, WMDockItem, item);
	Check_Type(signal_type, T_STRING);
	signal = malloc(sizeof(struct WMDockSignal));
	memset(signal, 0, sizeof(struct WMDockSignal));
	if (strcmp(StringValuePtr(signal_type), "button_press_event") == 0) {
		signal->type = ButtonPress;
	}  else if (strcmp(StringValuePtr(signal_type), 
			   "button_release_event") == 0) {
		signal->type = ButtonRelease;
	} else {
		exit(0);
	}
	signal->callback = rb_block_proc();

	if (item->signal == NULL) {
		item->signal = signal;
	} else {
		tmp = item->signal;
		while (tmp->next != NULL) {
			tmp = tmp->next;
		}
		tmp->next = signal;
	}
	signal->next = NULL;
}

static void dockitem_draw_point(VALUE self, VALUE x, VALUE y, VALUE color)
{
	WMDockItem *item;

	Data_Get_Struct(self, WMDockItem, item);
	
	Check_Type(x, T_FIXNUM);
	Check_Type(y, T_FIXNUM);
	Check_Type(color, T_STRING);

	draw_point(item->dock, item->x + FIX2INT(x),
		   item->y + FIX2INT(y), StringValuePtr(color));
}

static void dockitem_draw_line(VALUE self, VALUE x1, VALUE y1,
			VALUE x2, VALUE y2, VALUE color)
{
	WMDockItem *item;

	Data_Get_Struct(self, WMDockItem, item);
	
	Check_Type(x1, T_FIXNUM);
	Check_Type(y1, T_FIXNUM);
	Check_Type(x2, T_FIXNUM);
	Check_Type(y2, T_FIXNUM);
	Check_Type(color, T_STRING);

	draw_line(item->dock, item->x + FIX2INT(x1),
		  item->y + FIX2INT(y1),
		  item->x + FIX2INT(x2),
		  item->y + FIX2INT(y2),
		  StringValuePtr(color));
}

static void dockitem_draw_rect(VALUE self, VALUE x, VALUE y,
			VALUE width, VALUE height, VALUE color)
{
	WMDockItem *item;

	Data_Get_Struct(self, WMDockItem, item);
	
	Check_Type(x, T_FIXNUM);
	Check_Type(y, T_FIXNUM);
	Check_Type(width, T_FIXNUM);
	Check_Type(height, T_FIXNUM);
	Check_Type(color, T_STRING);

	draw_rect(item->dock, item->x + FIX2INT(x),
		  item->y + FIX2INT(y),
		  FIX2INT(width),
		  FIX2INT(height),
		  StringValuePtr(color));
}

static void dockitem_set_pixmap(VALUE self, VALUE filename)
{
	WMDockApp *dock;
	WMDockItem *dockitem;
	
	Check_Type(filename, T_STRING);

	Data_Get_Struct(self, WMDockItem, dockitem);
	dock = dockitem->dock;
	if (dock == NULL) {
		return;
	}
	GetXPMfromFile(&(dockitem->xpm), StringValuePtr(filename));
//	GetXPM(dock, &(dockitem->xpm), text_xpm);
	XCopyArea(dock->display, 
		  dockitem->xpm.pixmap,
		  dock->wmgen.pixmap, 
		  dock->NormalGC,
		  0, 0, dockitem->xpm.attributes.width,
		  dockitem->xpm.attributes.height,
		  dockitem->x, dockitem->y);
	RedrawWindow(dock);

}

static void dockitem_drawstring(int argc, VALUE *argv, VALUE self)
{
	WMDockApp *dock;
	WMDockItem *dockitem;
	VALUE x, y, text, vcolor;
	char *color;
	if (rb_scan_args(argc, argv, "31", &x, &y, &text, &vcolor) == 3) {
		color = strdup(TEXTCOLOR);
	} else {
		Check_Type(vcolor, T_STRING);
		color = strdup(StringValuePtr(vcolor));
	}
	Check_Type(text, T_STRING);
	Check_Type(x, T_FIXNUM);
	Check_Type(y, T_FIXNUM);

	Data_Get_Struct(self, WMDockItem, dockitem);
	dock = dockitem->dock;
	if (dock == NULL) {
		return;
	}
	drawString(dock, dockitem->x+FIX2INT(x), dockitem->y+FIX2INT(y), 
		   StringValuePtr(text), color, BGCOLOR, 0);
	RedrawWindow(dock);
	free(color);
}

/* Todo: color, font */
/* argv[0]: text string */
/* argv[1]: x */
/* argv[2]: y */
/* argv[3]: text type(0 = normal, 1 = Yellow text) */
static void dockitem_drawLEDstring(VALUE self, VALUE x, VALUE y,
				   VALUE text, VALUE color)
{
	WMDockApp *dock;
	WMDockItem *dockitem;
	char **lines;
	int i, dest_x, dest_y;

	Check_Type(text, T_STRING);
	Check_Type(x, T_FIXNUM);
	Check_Type(y, T_FIXNUM);
	Check_Type(color, T_FIXNUM);

	Data_Get_Struct(self, WMDockItem, dockitem);
	dock = dockitem->dock;
	if (dock == NULL) {
		return;
	}

	lines = strsplit(StringValuePtr(text), "\n", 0);
	dest_x = FIX2INT(x)+dockitem->x;
	dest_y = FIX2INT(y)+dockitem->y;
	for (i = 0; lines[i] != NULL; i++) {
		if (strcmp("", lines[i]) == 0) {
			break;
		}
		drawLEDString(dock, dest_x, dest_y, lines[i], FIX2INT(color));
		dest_y += 10;
	}
	if (lines)
	  free(lines);
}

void dockitem_callback(VALUE self)
{
	WMDockItem *item;

	Data_Get_Struct(self, WMDockItem, item);

	item->callback = rb_block_proc();

/*
	AddMouseRegion(mouse_region_index, item->x, item->y, 
		       item->x + item->width, item->y + item->height, 
		       item);
	mouse_region_index++;
*/
}

static void dockitem_clear(VALUE self)
{
	WMDockItem *item;

	Data_Get_Struct(self, WMDockItem, item);

	eraseRect(item->dock, item->x, item->y, item->x + item->width-1,
		  item->y + item->height, BGCOLOR);
	
}

void dockitem_mark(WMDockItem *item)
{
	rb_gc_mark(item->callback);
}

static VALUE dockitem_s_new(int argc, VALUE *argv, VALUE self)
{
	VALUE obj;
	VALUE width, height, rtype;
	WMDockItem *item;
	int type; 

	if (rb_scan_args(argc, argv, "21", &width, &height, &rtype) == 2) {
		type = ITEMTYPE_RECTANGLE;
	} else {
		if (strcmp(StringValuePtr(rtype), "circle") == 0) {
			type = ITEMTYPE_CIRCLE;
		} else {
			type = ITEMTYPE_RECTANGLE;
		}
	}

	Check_Type(width, T_FIXNUM);
	Check_Type(height, T_FIXNUM);
	
	item = malloc(sizeof(WMDockItem));
	memset(item, 0, sizeof(WMDockItem));

	item->text = NULL;
	item->width = FIX2INT(width);
	item->height = FIX2INT(height);
	item->type = type;
	obj = Data_Wrap_Struct(self, dockitem_mark, -1, item);
	return obj;
}

void dockitem_init(VALUE rb_DockApp)
{
	VALUE rb_DockItem;

	rb_DockItem = rb_define_class_under(rb_DockApp, "Item", rb_cObject);
	rb_define_singleton_method(rb_DockItem, "new",  dockitem_s_new, -1);
	rb_define_method(rb_DockItem, "drawLEDstring", 
			 RUBY_METHOD_FUNC(dockitem_drawLEDstring), 4);
	rb_define_method(rb_DockItem, "draw_string", 
			 RUBY_METHOD_FUNC(dockitem_drawstring), -1);
	rb_define_method(rb_DockItem, "click_callback",
			 RUBY_METHOD_FUNC(dockitem_callback), 0);
	rb_define_method(rb_DockItem, "clear",
			 RUBY_METHOD_FUNC(dockitem_clear), 0);
	rb_define_method(rb_DockItem, "set_pixmap",
			 RUBY_METHOD_FUNC(dockitem_set_pixmap), 1);
	rb_define_method(rb_DockItem, "draw_point",
			 RUBY_METHOD_FUNC(dockitem_draw_point), 3);
	rb_define_method(rb_DockItem, "draw_line",
			 RUBY_METHOD_FUNC(dockitem_draw_line), 5);
	rb_define_method(rb_DockItem, "draw_rect",
			 RUBY_METHOD_FUNC(dockitem_draw_rect), 5);

	rb_define_method(rb_DockItem, "signal_connect",
			 RUBY_METHOD_FUNC(dockitem_signal_connect), 1);
}
