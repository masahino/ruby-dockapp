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
	XpmIcon         parts_pixmap;
	XFontSet        fontset;
	Window		iconwin;
	Window          win;
	Pixmap		pixmask;
	char            *wname;
	char            **xpm_master;
	WMDockTimer     *timer;
	Bool            use_fontset;
	struct WMDockSignal *signal;
} WMDockApp;

struct WMDockSignal {
	void *next;
	int type;
	XEvent event;
	VALUE callback;
};

struct WMDockEvent {
	XEvent event;
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
	char            **xpm_master; /* test */
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
void docksignal_mark(struct WMDockSignal *signal);


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

/* dockpopup.h */
void dockpopup_init(VALUE rb_DockApp);

/* dockevent.h */
VALUE dockevent_initialize(VALUE self, XEvent xevent);
void dockevent_init(VALUE rb_DockApp);
#endif
