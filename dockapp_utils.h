#ifndef DOCKAPP_UTILS_H_INCLUDED
#define DOCKAPP_UTILS_H_INCLUDED
#include "dockapp.h"

/* dockapp_utils.c */
int get_Xsignal_type(char *signal_type);
int wait_Xevent(WMDockApp *dock, int event_type);
void get_pointer_position(Window win, int *x, int *y);
void draw_point(WMDockApp *dock, int x, int y, char *color);
void draw_line(WMDockApp *dock, int x1, int y1,
	       int x2, int y2, char *color);
void draw_line2(WMDockApp *dock, XpmIcon wmgen, int x1, int y1,
	       int x2, int y2, char *color);
void draw_rect(WMDockApp *dock, int x, int y,
	       int width, int height, char *color);
void draw_rect2(WMDockApp *dock, XpmIcon wmgen, int x, int y,
	       int width, int height, char *color);
char **strsplit(char *str, char *delimiter, int max);
void RedrawWindow(WMDockApp *dock);
void RedrawWindow2(Display *display, Pixmap src_pixmap, Window window,
		   GC gc, int width, int height);
void init_pixmap(WMDockApp *dock);
/*
void AddMouseRegion(int index, int left, int top, int right, int bottom,
		    WMDockItem *item);
*/
void AddMouseRegion(WMDockApp *dock, int left, int top, int right, int bottom, 
		    WMDockItem *item);
/* int CheckMouseRegion(int x, int y);*/
int CheckMouseRegion(WMDockApp *dock, int x, int y);
void openXwindow(int argc, char *argv[], char **, char *, int, int);
void RedrawWindowXY(int x, int y);
void copyXPMArea(int, int, int, int, int, int);
void copyXBMArea(int, int, int, int, int, int);
void setMaskXY(int, int);
void parse_rcfile(const char *, rckeys *);
void eraseRect(WMDockApp *dock, int x, int y, int x2, int y2,
	       char *bgcolorname);
void drawString(WMDockApp *dock, int dest_x, int dest_y, const char *string,
		char *colorname, char *bgcolorname,
		int right_justify);
void drawnString(WMDockApp *dock, int dest_x, int dest_y, const char *string,
		 char *colorname, char *bgcolorname,
		int right_justify, int len);
void drawLEDString(WMDockApp *dock, int dest_x, int dest_y, const char *string,
		   int color);
void drawnLEDString(WMDockApp *dock, int dest_x, int dest_y,
		    const char *string, int len, int color);
void drawnLEDString2(WMDockApp *dock, XpmIcon wmgen, int dest_x, int dest_y,
		     const char *string, int len, int color);
void set_pixmap(WMDockApp *dock, int x1, int y1, int x2, int y2);
void GetXPM(WMDockApp *dockapp, XpmIcon *wmgen, char *pixmap_bytes[]);
void GetXPM2(XpmIcon *wmgen, char *pixmap_bytes[]);
Pixel GetColor(WMDockApp *dock, char *name);
void createXBMfromXPM(char *xbm_org, char **xpm, int sx, int sy);
void GetXPMfromFile(XpmIcon *wmgen, char *filename);
void GetXPMfromData(XpmIcon *wmgen, char *pixmap_bytes[]);
void new_timer(WMDockTimer *timer, int interval);
void draw_ledpoint(WMDockApp *dock, int x, int y, char *color);
void set_pixmap_circle(WMDockApp *dock, int x1, int y1, int x2, int y2);
void mask_window2(Window window, char **xpm_master, int width, int height);
char** init_pixmap_with_size(int width, int height);

void hide_tooltip_window(Display *display, Window win);
Window create_tooltip_window(WMDockApp *dock);
void update_tooltip_window(WMDockApp *dock, Window win,
			   int x, int y, char *text);



#endif
