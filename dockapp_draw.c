/* */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdarg.h>
#include <X11/Xlib.h>
#include <X11/xpm.h>
#include <X11/extensions/shape.h>

#include "dockapp.h"
#include "dockapp_utils.h"

void draw_ledpoint(WMDockApp *dock, int x, int y, char *color)
{
	int src_x, src_y;
	if (strcmp("green", color) == 0) {
		src_x = 50;
		src_y =  6;
	} else if (strcmp("red", color) == 0) {
		src_x = 56;
		src_y =  1;
	} else if (strcmp("yellow", color) == 0) {
		src_x = 56;
		src_y =  6;
	} else if (strcmp("off", color) == 0) {
		src_x = 50;
		src_y =  1;
	} else {
		/* error */
		return;
	}
	XCopyArea(dock->display, 
		  dock->parts_pixmap.pixmap,
		  dock->wmgen.pixmap, 
		  dock->NormalGC,
		  src_x, src_y, 4, 4, x, y); // off

}

void draw_point(WMDockApp *dock, int x, int y, char *color)
{
        XSetForeground(dock->display, dock->NormalGC, 
		       GetColor(dock, color));
	XDrawPoint(dock->display, dock->wmgen.pixmap,
		   dock->NormalGC, x, y);
	RedrawWindow(dock);
}

void draw_line2(WMDockApp *dock, XpmIcon wmgen, int x1, int y1, int x2, int y2,
		char *color)
{
        XSetForeground(dock->display, dock->NormalGC, 
		       GetColor(dock, color));
	XDrawLine(dock->display, wmgen.pixmap,
		   dock->NormalGC, x1, y1, x2, y2);
	RedrawWindow(dock);
}

void draw_line(WMDockApp *dock, int x1, int y1,
	       int x2, int y2, char *color)
{
	draw_line2(dock, dock->wmgen, x1, y1, x2, y2, color);
}


void draw_rect2(WMDockApp *dock, XpmIcon wmgen, int x, int y,
	       int width, int height, char *color)
{
        XSetForeground(dock->display, dock->NormalGC, 
		       GetColor(dock, color));
	XFillRectangle(dock->display, wmgen.pixmap,
		       dock->NormalGC, x, y, width, height);
	RedrawWindow(dock);
}

void draw_rect(WMDockApp *dock, int x, int y,
	       int width, int height, char *color)
{
	draw_rect2(dock, dock->wmgen, x, y, width, height, color);
}

void drawnLEDString2(WMDockApp *dock, XpmIcon wmgen, int dest_x, int dest_y,
		    const char *string, int len, int color)
{
	int src_x = 0;
	int src_y = 0;
	int i;
	int color_offset = color * COLOR_OFFSET;
	for (i = 0; i < len; i++) {
		if (i >= len) {
			XCopyArea(dock->display, 
				  dock->text_pixmap.pixmap,
				  wmgen.pixmap, 
				  dock->NormalGC,
				  1, 27, 5, 9, dest_x, dest_y);
		} else {
			if (i > strlen(string)) {
				src_x = 1;
				src_y = LEDTEXT_SYMBOL_OFFSET*LEDCHAR_HEIGHT
					+ color_offset;
			} else if (isdigit(string[i])) {
				src_x = 1+(string[i]-'0')*6;
				src_y = 0 + color_offset;
			} else if (isalpha(string[i])) {
			  if (isupper(string[i])) {
				  src_x = 1+(string[i]-'A')*6;
			  } else {
				  src_x = 1+(string[i]-'a')*6;
			  }
				src_y = LEDTEXT_ALPHA_OFFSET*LEDCHAR_HEIGHT
					+ color_offset;
			} else if (string[i] == ':') {
				src_x = 1 + 6;
				src_y = LEDTEXT_SYMBOL_OFFSET*LEDCHAR_HEIGHT
					+ color_offset;
			} else if (string[i] == '.') {
				src_x = 1 + 12;
				src_y = LEDTEXT_SYMBOL_OFFSET*LEDCHAR_HEIGHT
					+ color_offset;
			} else if (string[i] == '/') {
				src_x = 1 + 18;
				src_y = LEDTEXT_SYMBOL_OFFSET*LEDCHAR_HEIGHT
					+ color_offset;
			} else if (string[i] == '%') {
				src_x = 1 + 24;
				src_y = LEDTEXT_SYMBOL_OFFSET*LEDCHAR_HEIGHT
					+ color_offset;
			} else {
				src_x = 1;
				src_y = LEDTEXT_SYMBOL_OFFSET*LEDCHAR_HEIGHT
					+ color_offset;
			}

			XCopyArea(dock->display, dock->text_pixmap.pixmap, 
				  wmgen.pixmap, dock->NormalGC,
				  src_x, src_y, 5, 9, dest_x, dest_y);

		}
		dest_x += 6;
	}
	/*
	RedrawWindow2(dock->display, wmgen.pixmap, dock->Root, dock->NormalGC, 64, 64);
	*/
	RedrawWindow(dock);
}

void drawnLEDString(WMDockApp *dock, int dest_x, int dest_y,
		    const char *string, int len, int color)
{
	drawnLEDString2(dock, dock->wmgen, dest_x, dest_y, string, len, color);
}
void drawLEDString(WMDockApp *dock, int dest_x, int dest_y, 
		   const char *string, int color)
{
	int len = strlen(string);
	drawnLEDString(dock, dest_x, dest_y, string, len, color);
}


void drawnString2(WMDockApp *dock, XpmIcon wmgen, 
		  int dest_x, int dest_y, const char *string,
		  char *colorname, char *bgcolorname,
		  int right_justify, int len)
{
        XSetForeground(dock->display, dock->NormalGC,
		       GetColor(dock, colorname));
        XSetBackground(dock->display, dock->NormalGC,
		       GetColor(dock, bgcolorname));
	if (dock->use_fontset) {
		XmbDrawString(dock->display, wmgen.pixmap,
			      dock->fontset, dock->NormalGC,
			      dest_x, dest_y, string, len);
	} else {
		XDrawString (dock->display, wmgen.pixmap,
			     dock->NormalGC, dest_x, dest_y, string, len);
	}
}

void drawnString(WMDockApp *dock, int dest_x, int dest_y, const char *string,
                                char *colorname, char *bgcolorname,
                                int right_justify, int len)
{
	drawnString2(dock, dock->wmgen, dest_x, dest_y, string,
		     colorname, bgcolorname, right_justify, len);
}

void drawString(WMDockApp *dock, int dest_x, int dest_y, const char *string,
		char *colorname, char *bgcolorname,
                                int right_justify)
{

        int len = strlen(string);
	drawnString(dock, dest_x, dest_y, string, colorname, bgcolorname,
		    right_justify, len);
}


