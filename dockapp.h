#ifndef DOCKAPP_H_INCLUDED
#define DOCKAPP_H_INCLUDED

/* Defines */
#define MAX_MOUSE_REGION (8)

#define DEFAULT_FONT "-shinonome-gothic-medium-r-normal--12-*-*-*-*-*"
#define SUBSTITUTE_FONT "-misc-fixed-medium-r-normal--12-*-*-*-*-*"

#define LEDCHAR_WIDTH (6)
#define LEDCHAR_HEIGHT (9)

#define LEDTEXT_MARGIN (4)
#define COLOR_OFFSET (27)
#define LEDTEXT_DIGIT_OFFSET (0)
#define LEDTEXT_ALPHA_OFFSET (1)
#define LEDTEXT_SYMBOL_OFFSET (2)
#define COLOR_NORMAL (0)

#define BGCOLOR "#208120812081"
#define TEXTCOLOR "#2081B2CAAEBA"

#define DOCKITEM_INVISIBLE 0
#define DOCKITEM_VISIBLE 1

/* Typedefs */
typedef struct _rckeys rckeys;

struct _rckeys {
	const char	*label;
	char		**var;
};

typedef struct {
	Pixmap		pixmap;
	Pixmap		mask;
	XpmAttributes	attributes;
} XpmIcon;

/* Global variable */
typedef struct {
	void   *next;
	VALUE  callback;
	struct timeval interval;
	struct timeval next_time;
	int status;
} WMDockTimer;

typedef struct {
	void            *item;
	Display         *display;
	Window          Root;
	GC              NormalGC;
	XpmIcon         wmgen;
	XpmIcon         text_pixmap;
	XFontSet        fontset;
	Window		iconwin;
	Window          win;
	Pixmap		pixmask;
	char            *wname;
	char            **xpm_master;
	WMDockTimer         *timer;
} WMDockApp;

typedef struct {
	void            *next;
	WMDockApp       *dock;
	char            *text;
	int             x;
	int             y;
	int             width;
	int             height;
	VALUE           callback;
	int             index; /* unused */
	int             visible;
	XpmIcon         xpm;
} WMDockItem;


/* Mouse Regions */
typedef struct {
	int	   enable;
	int	   top;
	int	   bottom;
	int	   left;
	int	   right;
	WMDockItem *item;
} MOUSE_REGION;

MOUSE_REGION	mouse_region[MAX_MOUSE_REGION];
#define MAX_LIST_LINE 64
int mouse_region_index;

/* Function Prototypes */
/* dockapp_utils.c */
void draw_point(WMDockApp *dock, int x, int y, char *color);
void draw_line(WMDockApp *dock, int x1, int y1,
	       int x2, int y2, char *color);
void draw_rect(WMDockApp *dock, int x, int y,
	       int width, int height, char *color);
char **strsplit(char *str, char *delimiter, int max);
void RedrawWindow(WMDockApp *dock);
void init_pixmap(WMDockApp *dock);
void AddMouseRegion(int index, int left, int top, int right, int bottom,
		    WMDockItem *item);
int CheckMouseRegion(int x, int y);
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
void set_pixmap(WMDockApp *dock, int x1, int y1, int x2, int y2);
void GetXPM(WMDockApp *dockapp, XpmIcon *wmgen, char *pixmap_bytes[]);
Pixel GetColor(WMDockApp *dock, char *name);
void createXBMfromXPM(char *xbm_org, char **xpm, int sx, int sy);
void GetXPMfromFile(WMDockApp *dockapp, XpmIcon *wmgen, char *filename);
void new_timer(WMDockTimer *timer, int interval);

/* docktext.c */
void docktext_set_text(int argc, VALUE *argv, VALUE self);
VALUE docktext_s_new(int argc, VALUE *argv, VALUE self);

/* dockitem.c */
VALUE dockitem_s_new(VALUE self, VALUE width, VALUE height);
void dockitem_callback(VALUE self);
void dockitem_clear(VALUE self);
void dockitem_drawLEDstring(VALUE self, VALUE x, VALUE y, VALUE text, 
			  VALUE color);
void dockitem_drawstring(int argc, VALUE *argv, VALUE self);
void dockitem_set_timer(VALUE self, VALUE interval);
void dockitem_set_pixmap(VALUE self, VALUE filename);
void dockitem_draw_point(VALUE self, VALUE x, VALUE y, VALUE color);
void dockitem_draw_line(VALUE self, VALUE x1, VALUE y1,
			VALUE x2, VALUE y2, VALUE color);
void dockitem_draw_rect(VALUE self, VALUE x, VALUE y,
			VALUE width, VALUE height, VALUE color);
void dockitem_mark(WMDockItem *item);

/* docktimer.c */
VALUE docktimer_initialize(VALUE self, VALUE interval);
void update_timer(WMDockTimer *timer);
void docktimer_start(VALUE self);
void docktimer_stop(VALUE self);
VALUE docktimer_getstatus(VALUE self);

#endif
