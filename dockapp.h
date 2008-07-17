#ifndef DOCKAPP_H_INCLUDED
#define DOCKAPP_H_INCLUDED


#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/xpm.h>

#include <ruby.h>

/* Defines */
#define RUBY_DOCKAPP_VERSION "0.2.3"

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

typedef enum {
     DockAppStatusDestroy,
     DockAppStatusOpen,
     DockAppStatusRun,
} DockAppStatus;

typedef enum {
     ItemType_Item,
     ItemType_Text,
     ItemType_PopUp_Led,
     ItemType_PopUp_Text,
     ItemType_PopUp_Image,
} ItemType;

typedef enum {
     DockItemShape_Box,
     DockItemShape_Circle,
} DockItemShape;

typedef enum {
     DockItemStyle_Normal,
     DockItemStyle_Button,
} DockItemStyle;

/* Typedefs */

struct _rckeys {
     const char	*label;
     char		**var;
};

/* Mouse Regions */
typedef struct {
     int	   enable;
     int	   top;
     int	   bottom;
     int	   left;
     int	   right;
     void *item;
} MOUSE_REGION2;


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
     MOUSE_REGION2   mouse_region[MAX_MOUSE_REGION];
     int mouse_region_index;
     int             status;
/*	void            *focused_item;*/
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
     void                *next;
     WMDockApp           *dock;
     struct WMDockSignal *signal;
     VALUE               callback; /* use signal */
     void                (*redraw_function)();
     XpmIcon             xpm;
     Window              win;
     char                **xpm_master; /* test */
     char                *text; /* DockApp::Text */
     int                 x;
     int                 y;
     int                 width;
     int                 height;
     int                 visible;
     int                 type;
     int                 shape; /* item option */
     int                 style; /* item option */
     char                *tip_text;
     void                *option;
} WMDockItem;

typedef struct {
     int shape;
     int style;
     char *bgcolor;
} WMDockItemOption;

typedef struct {
     int item_num;
     int row_height;
} WMDockPopUpOption;

typedef struct {
     char *text;
} WMDockTextOption;

#if 0
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
int mouse_region_index;
#endif

#define MAX_LIST_LINE 64
/*Display         *display;*/
/*Window          Root;*/
//GC              NormalGC;

/*ID id_call;*/

/*VALUE rb_DockItem;*/

/* Function Prototypes */
void docksignal_mark(struct WMDockSignal *signal);


/* docktext.c */
void docktext_init(VALUE rb_DockApp);
void redraw_docktext(WMDockItem *item);

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
