#ifndef DOCKAPP_DRAW_H
#define DOCKAPP_DRAW_H
#include "dockapp.h"

void draw_ledpoint(WMDockApp *dock, int x, int y, char *color);
void draw_point(WMDockApp *dock, int x, int y, char *color);
void draw_line(WMDockApp *dock, int x1, int y1,
	       int x2, int y2, char *color);
void draw_line2(WMDockApp *dock, XpmIcon wmgen, int x1, int y1,
	       int x2, int y2, char *color);
void draw_rect(WMDockApp *dock, int x, int y,
	       int width, int height, char *color);
void draw_rect2(WMDockApp *dock, XpmIcon wmgen, int x, int y,
	       int width, int height, char *color);
void drawString(WMDockApp *dock, int dest_x, int dest_y, const char *string,
		char *colorname, char *bgcolorname,
		int right_justify);
void drawnString(WMDockApp *dock, int dest_x, int dest_y, const char *string,
		 char *colorname, char *bgcolorname,
		int right_justify, int len);
void drawnString2(WMDockApp *dock, XpmIcon wmgen, 
		  int dest_x, int dest_y, const char *string,
		  char *colorname, char *bgcolorname,
		  int right_justify, int len);
void drawLEDString(WMDockApp *dock, int dest_x, int dest_y, const char *string,
		   int color);
void drawnLEDString(WMDockApp *dock, int dest_x, int dest_y,
		    const char *string, int len, int color);
void drawnLEDString2(WMDockApp *dock, XpmIcon wmgen, int dest_x, int dest_y,
		     const char *string, int len, int color);

#endif /* DOCKAPP_DRAW_H */
