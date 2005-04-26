#ifndef DOCKAPP_H_INCLUDED
#define DOCKAPP_H_INCLUDED

/* Defines */
#define MAX_MOUSE_REGION (16)

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
#define ITEMTYPE_RECTANGLE 0
#define ITEMTYPE_CIRCLE 1

#define TYPE_POPUP 10

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
	WMDockTimer     *timer;
	Bool            use_fontset;
} WMDockApp;

struct WMDockSignal {
	void *next;
	int type;
	VALUE callback;
};

typedef struct {
	void            *next;
	WMDockApp       *dock;
	char            *text;
	int             x;
	int             y;
	int             width;
	int             height;
	VALUE           callback;
	struct WMDockSignal *signal;
	int             index; /* unused */
	int             visible;
	XpmIcon         xpm;
	int             type;
	Window          win;
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
Display         *display;
Window          Root;
GC              NormalGC;


/* Function Prototypes */
/* dockapp_utils.c */
void get_pointer_position(Window win, int *x, int *y);
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
void GetXPMfromFile(XpmIcon *wmgen, char *filename);
void new_timer(WMDockTimer *timer, int interval);

/* docktext.c */
void docktext_init(VALUE rb_DockApp);

/* dockitem.c */
void dockitem_callback(VALUE self);
void dockitem_mark(WMDockItem *item);
void dockitem_init(VALUE rb_DockApp);


/* docktimer.c */
void docktimer_init(VALUE rb_DockApp);
void update_timer(WMDockTimer *timer);

#if 0
void gtk_dockapp_init();
#endif 

void dockpopup_init(VALUE rb_DockApp);

#endif
