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
//#include "pixmap.h"

static void set_text(WMDockItem *dockitem, char *text, int color)
{
	char **lines;
	int i, dest_x, dest_y;
	int row, column;
	WMDockApp *dock;
	dock = dockitem->dock;
	if (dock == NULL) {
		return;
	}
	
	row = (dockitem->height-LEDTEXT_MARGIN)/LEDCHAR_HEIGHT;
	column = (dockitem->width-LEDTEXT_MARGIN)/LEDCHAR_WIDTH;

	lines = strsplit(text, "\n", 0);
	dest_x = dockitem->x + 2;
	dest_y = dockitem->y + 2;
	for (i = 0; lines[i] != NULL; i++) {
		if (strcmp("", lines[i]) == 0) {
			break;
		}
		if (i > row) {
			break;
		}
		drawnLEDString(dock, dest_x, dest_y, lines[i], 
			       column,
			       color);
		dest_y += LEDCHAR_HEIGHT;
	}
	if (lines)
	  free(lines);
}

/*void docktext_set_text(VALUE self, VALUE text, VALUE color)*/
void docktext_set_text(int argc, VALUE *argv, VALUE self)
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
	set_text(dockitem, StringValuePtr(text), FIX2INT(color));
	if (strcmp(dockitem->text, StringValuePtr(text)) != 0) {
		free(dockitem->text);
		dockitem->text = strdup(StringValuePtr(text));
	}
}


VALUE docktext_s_new(int argc, VALUE *argv, VALUE self)
{
	VALUE obj;
	WMDockItem *wmtext;
	VALUE text, vcolor, width, height;
	int color;

	if (rb_scan_args(argc, argv, "31", &text, &width, &height, &vcolor) == 3) {
		color = 0;
	} else {
		Check_Type(vcolor, T_FIXNUM);
		color = FIX2INT(vcolor);
	}
	Check_Type(text, T_STRING);
	Check_Type(width, T_FIXNUM);
	Check_Type(height, T_FIXNUM);
	  
	wmtext = malloc(sizeof(WMDockItem));
	memset(wmtext, 0, sizeof(WMDockItem));
	wmtext->text = strdup(StringValuePtr(text));
	wmtext->width = FIX2INT(width)*LEDCHAR_WIDTH + LEDTEXT_MARGIN;
	wmtext->height = FIX2INT(height)*LEDCHAR_HEIGHT + LEDTEXT_MARGIN;
	obj = Data_Wrap_Struct(self, dockitem_mark, -1, wmtext);

	set_text(wmtext, wmtext->text, color);
	
	return obj;
}
