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
#include "dockapp_utils.h"
#include "dockapp_draw.h"

extern VALUE rb_DockItem;


static void set_text(WMDockItem *dockitem, char *text, int color)
{
	char **lines;
	int i, dest_x, dest_y;
	int row, column;
	WMDockApp *dock;

	dock = dockitem->dock;
	if (dock == NULL) {
/*		printf ("dock == NULL\n"); */
		return;
	}
	
	row = (dockitem->height-LEDTEXT_MARGIN)/LEDCHAR_HEIGHT;
	column = (dockitem->width-LEDTEXT_MARGIN)/LEDCHAR_WIDTH;
	lines = strsplit(text, "\n", 0);
	dest_x = dockitem->x + 2;
	dest_y = dockitem->y + 2;
	
/* 	printf ("row == %d\n", row); */
	for (i = 0; i < row; i++) {
		if (strcmp("", lines[i]) == 0) {
			drawnLEDString(dock, dest_x, dest_y, "",
				       column, color);
			break;
		}
		drawnLEDString(dock, dest_x, dest_y, lines[i], 
			       column,
			       color);
		dest_y += LEDCHAR_HEIGHT;
	}
	if (lines) {
		free(lines);
	}
}

void redraw_docktext(WMDockItem *dockitem)
{
	set_text(dockitem, dockitem->text, COLOR_NORMAL);
}

static void docktext_set_text(int argc, VALUE *argv, VALUE self)
{
	WMDockItem *dockitem;
	VALUE text, vcolor;
	int color = COLOR_NORMAL;

	if (rb_scan_args(argc, argv, "11", &text, &vcolor) == 1) {
		color = COLOR_NORMAL;
	} else {
		Check_Type(color, T_FIXNUM);
		color = FIX2INT(vcolor);
	}
	Check_Type(text, T_STRING);

	Data_Get_Struct(self, WMDockItem, dockitem);
	set_text(dockitem, StringValuePtr(text), color);
	if (strcmp(dockitem->text, StringValuePtr(text)) != 0) {
		free(dockitem->text);
		dockitem->text = strdup(StringValuePtr(text));
	}
}


static VALUE docktext_s_new(int argc, VALUE *argv, VALUE self)
{
	VALUE obj;
	WMDockItem *wmtext;
	VALUE text, vcolor, width, height;
	int color;

	if (rb_scan_args(argc, argv, "31", &text, &width, &height, &vcolor) == 3) {
		color = COLOR_NORMAL;
	} else {
		Check_Type(vcolor, T_FIXNUM);
		color = FIX2INT(vcolor);
	}
	Check_Type(text, T_STRING);
	Check_Type(width, T_FIXNUM);
	Check_Type(height, T_FIXNUM);
	  
	wmtext = ALLOC(WMDockItem);
	memset(wmtext, 0, sizeof(WMDockItem));
	wmtext->text = strdup(StringValuePtr(text));
	wmtext->width = FIX2INT(width)*LEDCHAR_WIDTH + LEDTEXT_MARGIN;
	wmtext->height = FIX2INT(height)*LEDCHAR_HEIGHT + LEDTEXT_MARGIN;
	wmtext->redraw_function = &redraw_docktext;
	wmtext->type = ItemType_Text;
	wmtext->shape = DockItemShape_Box;
	wmtext->style = DockItemStyle_Normal;
	obj = Data_Wrap_Struct(self, dockitem_mark, -1, wmtext);

	set_text(wmtext, wmtext->text, color);
	
	return obj;
}

void docktext_init(VALUE rb_DockApp)
{
	VALUE rb_DockText;

	rb_DockText = rb_define_class_under(rb_DockApp, "Text", rb_DockItem);
	rb_define_singleton_method(rb_DockText, "new", docktext_s_new, -1);
	rb_define_method(rb_DockText, "set_text",
			 RUBY_METHOD_FUNC(docktext_set_text), -1);
}
