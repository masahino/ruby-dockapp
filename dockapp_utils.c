/*	wmgeneral was taken from wmppp.

	It has a lot of routines which most of the wm* programs use.

	------------------------------------------------------------
	Author: Martijn Pieterse (pieterse@xs4all.nl)

	--- CHANGES: ---
	02/05/1998 (Martijn Pieterse, pieterse@xs4all.nl)
		* changed the read_rc_file to parse_rcfile, as suggester by Marcelo E. Magallon
		* debugged the parse_rc file.
	30/04/1998 (Martijn Pieterse, pieterse@xs4all.nl)
		* Ripped similar code from all the wm* programs,
		  and put them in a single file.
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
//#include "pixmap.h"

/* Function Prototypes */

void draw_point(WMDockApp *dock, int x, int y, char *color)
{
        XSetForeground(dock->display, dock->NormalGC, 
		       GetColor(dock, color));
	XDrawPoint(dock->display, dock->wmgen.pixmap,
		   dock->NormalGC, x, y);
	RedrawWindow(dock);
}

void draw_line(WMDockApp *dock, int x1, int y1,
	       int x2, int y2, char *color)
{
        XSetForeground(dock->display, dock->NormalGC, 
		       GetColor(dock, color));
	XDrawLine(dock->display, dock->wmgen.pixmap,
		   dock->NormalGC, x1, y1, x2, y2);
	RedrawWindow(dock);
}

void draw_rect(WMDockApp *dock, int x, int y,
	       int width, int height, char *color)
{
        XSetForeground(dock->display, dock->NormalGC, 
		       GetColor(dock, color));
	XFillRectangle(dock->display, dock->wmgen.pixmap,
		       dock->NormalGC, x, y, width, height);
	RedrawWindow(dock);
}

/*****************************************************************************\
|* createXBMfromXPM                                                          *|
\*****************************************************************************/
void createXBMfromXPM(char *xbm_org, char **xpm, int sx, int sy)
{

	int i, j, k;
	int width, height, numcol, depth;
	int zero = 0;
	unsigned char bwrite;
	int bcount;
	int curpixel;
	char *xbm;
	xbm = xbm_org;

	sscanf(*xpm, "%d %d %d %d", &width, &height, &numcol, &depth);

	for (k = 0; k != depth; k++) {
		zero <<= 8;
		zero |= xpm[1][k];
	}

	for (i = numcol + 1; i < numcol + sy + 1; i++) {
		bcount = 0;
		bwrite = 0;
		for (j = 0; j < sx * depth; j += depth) {
			bwrite >>= 1;

			curpixel = 0;
			for (k = 0; k != depth; k++) {
				curpixel <<= 8;
				curpixel |= xpm[i][j + k];
			}

			if (curpixel != zero) {
				bwrite += 128;
			}
			bcount++;
			if (bcount == 8) {
				*xbm = bwrite;
				xbm++;
				bcount = 0;
				bwrite = 0;
			}
		}
	}

}

char **strsplit(char *str, char *delimiter, int max)
{
	char **str_array;
	char *reminder;
	char *s;
	int n = 0;

	str_array = malloc(sizeof(char*)*MAX_LIST_LINE);
	reminder = str;
	s = strstr(reminder, delimiter);
	while(s) {
		int len;
		char *new_string;

		len = s - reminder;
		new_string = malloc(sizeof(char)*(len + 1));
    
		strncpy(new_string, reminder, len);
		new_string[len] = '\0';
		str_array[n] = new_string;
		// listに登録
		n++;
		reminder = s + 1;
		s = strstr(reminder, delimiter);
	}
	if (s)
		free(s);

	str_array[n] = reminder;
	str_array[n+1] = "";
		
		return str_array;

}

void eraseRect(WMDockApp *dock, int x, int y, int x2, int y2,
	       char *bgcolorname)
{
        XSetForeground(dock->display, dock->NormalGC, 
		       GetColor(dock, bgcolorname));
        XFillRectangle(dock->display, dock->wmgen.pixmap, 
		       dock->NormalGC, x, y, x2 - x, y2 - y);
}

void drawLEDString(WMDockApp *dock, int dest_x, int dest_y, 
		   const char *string, int color)
{
	int len = strlen(string);
	drawnLEDString(dock, dest_x, dest_y, string, len, color);
}

void drawnLEDString(WMDockApp *dock, int dest_x, int dest_y,
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
				  dock->wmgen.pixmap, 
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
				  dock->wmgen.pixmap, dock->NormalGC,
				  src_x,  src_y, 5, 9, dest_x, dest_y);
		}
		dest_x += 6;
	}
	RedrawWindow(dock);
}

void drawnString(WMDockApp *dock, int dest_x, int dest_y, const char *string,
                                char *colorname, char *bgcolorname,
                                int right_justify, int len)
{
        XSetForeground(dock->display, dock->NormalGC,
		       GetColor(dock, colorname));
        XSetBackground(dock->display, dock->NormalGC,
		       GetColor(dock, bgcolorname));
/*
        if (right_justify)
                dest_x -= XTextWidth(f, string, len);
*/
	if (dock->use_fontset) {
		XmbDrawString(dock->display, dock->wmgen.pixmap,
			      dock->fontset, dock->NormalGC,
			      dest_x, dest_y, string, len);
	} else {
		XDrawString (dock->display, dock->wmgen.pixmap,
			     dock->NormalGC, dest_x, dest_y, string, len);
	}
}

void drawString(WMDockApp *dock, int dest_x, int dest_y, const char *string,
		char *colorname, char *bgcolorname,
                                int right_justify)
{

        int len = strlen(string);
	drawnString(dock, dest_x, dest_y, string, colorname, bgcolorname,
		    right_justify, len);
}


/* read_rc_file */
void parse_rcfile(const char *filename, rckeys *keys)
{
	char	*p;
	char	temp[128];
	char	*tokens = " :	\n";
	FILE	*fp;
	int	i,key;

	fp = fopen(filename, "r");
	if (fp) {
		while (fgets(temp, 128, fp)) {
			key = 0;
				while (key >= 0 && keys[key].label) {
					if ((p = strstr(temp, keys[key].label))) {
						p += strlen(keys[key].label);
						p += strspn(p, tokens);
						if ((i = strcspn(p, "#\n"))) p[i] = 0;
						free(*keys[key].var);
						*keys[key].var = strdup(p);
						key = -1;
					} else key++;
				}
			}
		fclose(fp);
	}
}

void AddMouseRegion(int index, int left, int top, int right, int bottom, 
		    WMDockItem *item)
{
#ifdef DEBUG
	printf ("index = %d, left = %d, top = %d, right = %d, bottom = %d\n",
		index, left, top, right, bottom);
#endif /* DEBUG */
	if (index < MAX_MOUSE_REGION) {
		mouse_region[index].enable = 1;
		mouse_region[index].top = top;
		mouse_region[index].left = left;
		mouse_region[index].bottom = bottom;
		mouse_region[index].right = right;
		mouse_region[index].item = item;
	}
}

int CheckMouseRegion(int x, int y)
{
	int	i;

	for (i=0; i<MAX_MOUSE_REGION; i++) {
		if (mouse_region[i].enable &&
			x <= mouse_region[i].right &&
			x >= mouse_region[i].left &&
			y <= mouse_region[i].bottom &&
			y >= mouse_region[i].top)
/*			return (i-1);*/
		     return i;
	}
	return -1;
}

/*
void copyXPMArea(int x, int y, int sx, int sy, int dx, int dy)
{
	XCopyArea(display, wmgen.pixmap, wmgen.pixmap, NormalGC, x, y, sx, sy, dx, dy);
}

void copyXBMArea(int x, int y, int sx, int sy, int dx, int dy)
{
	XCopyArea(display, wmgen.mask, wmgen.pixmap, NormalGC, x, y, sx, sy, dx, dy);
}
*/


Pixel GetColor(WMDockApp *dock, char *name)
{
	XColor		color;
	XWindowAttributes	attributes;

	XGetWindowAttributes(dock->display, dock->Root, &attributes);

	color.pixel = 0;
	if (!XParseColor(dock->display, attributes.colormap, name, &color)) {
		fprintf(stderr, "wm.app: can't parse %s.\n", name);
	} else if (!XAllocColor(dock->display, attributes.colormap, &color)) {
		fprintf(stderr, "wm.app: can't allocate %s.\n", name);
	}
	return color.pixel;
}

static int flush_expose(Display *display, Window w)
{
	XEvent	dummy;
	int		i=0;

	while (XCheckTypedWindowEvent(display, w, Expose, &dummy))
		i++;
	return i;
}

void RedrawWindow(WMDockApp *dock)
{
	flush_expose(dock->display, dock->iconwin);
	XCopyArea(dock->display, dock->wmgen.pixmap, dock->iconwin, 
		  dock->NormalGC, 0, 0, dock->wmgen.attributes.width, 
		  dock->wmgen.attributes.height, 0,0);
	flush_expose(dock->display, dock->win);
	XCopyArea(dock->display, dock->wmgen.pixmap, dock->win, dock->NormalGC,
		0,0, dock->wmgen.attributes.width, 
		  dock->wmgen.attributes.height, 0,0);
}

void GetXPM(WMDockApp *dockapp, XpmIcon *wmgen, char *pixmap_bytes[])
{
	XWindowAttributes	attributes;
	int			err;

	/* For the colormap */
	XGetWindowAttributes(dockapp->display, dockapp->Root, &attributes);
	wmgen->attributes.valuemask |= (XpmReturnPixels | XpmReturnExtensions);

	err = XpmCreatePixmapFromData(dockapp->display, dockapp->Root, 
				      pixmap_bytes, &(wmgen->pixmap),
		&(wmgen->mask), &(wmgen->attributes));

	if (err != XpmSuccess) {
		fprintf(stderr, "Not enough free colorcells.\n");
		exit(1);
	}
}

void GetXPMfromFile(WMDockApp *dockapp, XpmIcon *wmgen, char *filename)
{
	XWindowAttributes	attributes;
	int			err;

	/* For the colormap */

	XGetWindowAttributes(dockapp->display, dockapp->Root, &attributes);
	wmgen->attributes.valuemask |= (XpmReturnPixels | XpmReturnExtensions);

	err = XpmReadFileToPixmap(dockapp->display, dockapp->Root, 
				  filename, &(wmgen->pixmap),
				  &(wmgen->mask), &(wmgen->attributes));
	printf ("%d\n", err);
	if (err != XpmSuccess) {
		fprintf(stderr, "Not enough free colorcells.\n");
		exit(1);
	}

}



void _setMaskXY(WMDockApp *dock, int x, int y)
{
	XShapeCombineMask(dock->display, dock->win, ShapeBounding, x, y, 
			  dock->pixmask, ShapeSet);
	XShapeCombineMask(dock->display, dock->iconwin, ShapeBounding, x, y,
			  dock->pixmask, ShapeSet);
}




static void mask_window(WMDockApp *dock)
{
	int pixmask_width = 64;
	int pixmask_height = 64;

	Pixmap pixmask;

	char mask_bits[64 * 64];

	createXBMfromXPM(mask_bits, dock->xpm_master, 64, 64);

	pixmask = XCreateBitmapFromData(dock->display, dock->win, 
					mask_bits, pixmask_width, 
					pixmask_height);

	XShapeCombineMask(dock->display, dock->win, ShapeBounding,
			  0, 0, pixmask, ShapeSet);
	XShapeCombineMask(dock->display, dock->iconwin, 
			  ShapeBounding, 0, 0, pixmask, ShapeSet);


}

void set_pixmap(WMDockApp *dock, int x1, int y1, int x2, int y2)
{
	int i;
	int width = 64;
	int height = 64;
	int margin = 4;
	int colors = 5;
	int base = colors + 1;

#ifdef DEBUG
	printf ("x1 = %d, y1 = %d, x2 = %d, y2 = %d\n", 
		x1, y1, x2, y2);
#endif /* DEBUG */

	for (i = base + margin; i < height + base - margin; i++) {
/*
                x1      x2
		+++++++++  y1
		+       @
		@@@@@@@@@  y2
*/
/* あふれた場合の処理を追加しないとヤバい */
		if ((i - base) == y1) {
			int j;
			for (j = x1;
			     j < x2; j++) {
				if (dock->xpm_master[i][j] == ' ') {
					dock->xpm_master[i][j] = '+';
				} else if (dock->xpm_master[i][j] == '@') {
					dock->xpm_master[i][j] = '.';
				}
				if (j > width) {
					break;
				}
			}
		} else if (y1 < (i - base) &&
			   (i - base) < y2) {
			if (dock->xpm_master[i][x1] == ' ') {
				dock->xpm_master[i][x1] = '+';
			} else if (dock->xpm_master[i][x1] == '@') {
				dock->xpm_master[i][x1] = '.';
			}				
/*			memset(dock->xpm_master[i] + margin + x1, '+', 1);*/
			memset(dock->xpm_master[i] + x1 + 1,
			       '.', x2 - x1 - 2);
			memset(dock->xpm_master[i] + x2 - 1, '@', 1);
		} else if ((i - base) == y2) {
			memset(dock->xpm_master[i] + x1, '@',
			       x2 - x1);
		}
			
	}
	GetXPM(dock, &dock->wmgen, dock->xpm_master);
	mask_window(dock);
#ifdef DEBUG
	{
		int i, j;
		for (i = 0; i < base + height; i++) {
			for (j = 0; j < width; j++) {
				printf ("%c", dock->xpm_master[i][j]);
//			printf ("%02d\"%s\"\n", i, dock->xpm_master[i]);
			}
			printf ("\n");
		}
	}
#endif /* DEBUG */
}

void init_pixmap(WMDockApp *dock)
{
	char **ret = malloc(sizeof(char *) * (64+6) +sizeof(void *));
	int i;
	int width = 64;
	int height = 64;
/*	int margin = 4;*/
	int colors = 5;
	int base = colors + 1;
	const char *background = "#202020";     /* background gray */

	ret[0] = malloc(30);
	sprintf(ret[0], "%d %d %d %d", 64, 64, 5, 1);
	ret[1] = (char *) " \tc #0000FF";	/* no color */
	ret[2] = (char *) ".\tc #202020";	/* background gray */
	ret[2] = malloc(30);
	sprintf(ret[2], ".\tc %s", background);
	ret[3] = (char *) "+\tc #000000";	/* shadowed */
	ret[4] = (char *) "@\tc #C7C3C7";	/* highlight */
	ret[5] = (char *) ":\tc #004941";	/* led off */
	for (i = base; i < base + height; i++) {
		ret[i] = malloc(width);
		memset(ret[i], ' ', width);
	}
	ret[height + base] = NULL;/* not sure if this is necessary, it just
				     seemed like a good idea  */
	dock->xpm_master = ret;
/*	GetXPM(dock, &dock->wmgen, ret);*/


#ifdef DEBUG
	/* dump pixmap information */
	{
		int i;
		for (i = 0; i < base + height; i++) {
			printf ("%02d\"%s\"\n", i, ret[i]);
		}
	}
#endif
}

