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

#include "dockapp.h"
#include "dockapp_utils.h"

void hide_tooltip_window(Display *display, Window win)
{
	XUnmapWindow(display, win);
}

Window create_tooltip_window(WMDockApp *dock)
{
	Window win;
	XSetWindowAttributes att;
	int padding;

	padding = 2;

	win = XCreateSimpleWindow(dock->display, dock->Root, 
				  0, 0,
				  1, 1,
				  1, BlackPixel(dock->display, 0), 
				  WhitePixel(dock->display, 0));
	XSetWindowBackground(dock->display, win, 
		       GetColor(dock, "#ffffc0"));
	XClearWindow(dock->display, win);
	att.override_redirect=True;
	XChangeWindowAttributes (dock->display, win,
				 CWOverrideRedirect, &att);
	return win;
}

void update_tooltip_window(WMDockApp *dock, Window win,
			     int x, int y, char *text)
{
	int width, height;
	XRectangle overall_ink, overall_logical;
	int padding;

	padding = 2;

	XmbTextExtents(dock->fontset, text, strlen(text), &overall_ink, 
		       &overall_logical);
	width = overall_logical.width + padding*2;
	height = overall_logical.height + padding*2;

	XResizeWindow(dock->display, win, width, height);
	if (x + width > DisplayWidth(dock->display, 0)) {
		x = DisplayWidth(dock->display, 0) - width - 10;
	}
	if (y + height > DisplayHeight(dock->display, 0)) {
		y = y - height - 16;
	}

	XMoveWindow(dock->display, win, x, y);

        XSetForeground(dock->display, dock->NormalGC,
		       GetColor(dock, "black"));
        XSetBackground(dock->display, dock->NormalGC,
		       GetColor(dock, "#ffffc0"));
#ifdef DEBUG
	printf ("%s\n", text);
#endif /* DEBUG */
	XMapWindow(dock->display, win);
	XmbDrawString(dock->display, win,
		      dock->fontset, dock->NormalGC,
		      padding, height-padding*2, text, strlen(text));
	XRaiseWindow(dock->display, win);
}
