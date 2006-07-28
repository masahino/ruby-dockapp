#ifndef DOCKAPP_UTILS_H_INCLUDED
#define DOCKAPP_UTILS_H_INCLUDED
#include "dockapp.h"

/* dockapp_utils.c */
void get_pointer_position(Window win, int *root_x, int *root_y, int *win_x, int *win_y);
char **strsplit(char *str, char *delimiter, int max);
void RedrawWindow(WMDockApp *dock);
void RedrawWindow2(Display *display, Pixmap src_pixmap, Window window,
		   GC gc, int width, int height);
void init_pixmap(WMDockApp *dock);
void AddMouseRegion(WMDockApp *dock, int left, int top, int right, int bottom, 
		    WMDockItem *item);
int CheckMouseRegion(WMDockApp *dock, int x, int y);
void openXwindow(int argc, char *argv[], char **, char *, int, int);
void RedrawWindowXY(int x, int y);
void copyXPMArea(int, int, int, int, int, int);
void copyXBMArea(int, int, int, int, int, int);
void setMaskXY(int, int);
void eraseRect(WMDockApp *dock, int x, int y, int x2, int y2,
	       char *bgcolorname);
void set_pixmap(WMDockApp *dock, int x1, int y1, int x2, int y2);
void GetXPM(WMDockApp *dockapp, XpmIcon *wmgen, char *pixmap_bytes[]);
void GetXPM2(XpmIcon *wmgen, char *pixmap_bytes[]);
Pixel GetColor(WMDockApp *dock, char *name);
void createXBMfromXPM(char *xbm_org, char **xpm, int sx, int sy);
void GetXPMfromFile(XpmIcon *wmgen, char *filename);
void GetXPMfromData(XpmIcon *wmgen, char *pixmap_bytes[]);
void new_timer(WMDockTimer *timer, int interval);
void set_pixmap_circle(WMDockApp *dock, int x1, int y1, int x2, int y2);
void set_pixmap_button(WMDockApp *dock, int x1, int y1, int x2, int y2);
void mask_window2(Window window, char **xpm_master, int width, int height);
char** init_pixmap_with_size(int width, int height);
char** init_pixmap_with_size_and_background(int width, int height,
					    char background_char);

#endif
