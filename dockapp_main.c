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
#include <signal.h>
#include <sys/time.h>
#include <time.h>

#include <X11/Xlib.h>
#include <X11/xpm.h>
#include <X11/extensions/shape.h>
#include <X11/Xlocale.h>

#include "ruby.h"

#include "dockapp.h"
#include "dockapp_utils.h"
#include "pixmap.h"

int u1 = 0;
int u2 = 0;

ID id_call;
ID id_relative_callbacks;
int timer_id = 0;
VALUE mDockApp;
extern WMDockTimer *docktimer;

/* from ruby-gnome2 */
/*
static void rbgobj_add_relative_removable(VALUE obj, VALUE relative, 
					  ID obj_ivar_id, VALUE hash_key)
{
	VALUE hash = Qnil;

	if (RTEST(rb_ivar_defined(obj, obj_ivar_id)))
		hash = rb_ivar_get(obj, obj_ivar_id);

	if (NIL_P(hash) || TYPE(hash) != T_HASH) {
		hash = rb_hash_new();
		rb_ivar_set(obj, obj_ivar_id, hash);
	}
	rb_hash_aset(hash, hash_key, relative);
}
*/

static void sig_int()
{
	rb_raise(rb_eInterrupt, "");
}

void dockapp_mark(WMDockApp *dock)
{
	if (dock->signal) {
		struct WMDockSignal *signal;
		signal = dock->signal;
		while (signal) {
			rb_gc_mark(signal->callback);
			signal = signal->next;
		}
	}
}

static void dockapp_signal_connect(VALUE self, VALUE signal_type)
{

	WMDockApp *dock;
	struct WMDockSignal *signal, *tmp;

	Data_Get_Struct(self, WMDockApp, dock);
	Check_Type(signal_type, T_STRING);
	signal = malloc(sizeof(struct WMDockSignal));
	memset(signal, 0, sizeof(struct WMDockSignal));
	if (strcmp(StringValuePtr(signal_type), "button_press_event") == 0) {
		signal->type = ButtonPress;
	}  else if (strcmp(StringValuePtr(signal_type), 
			   "button_release_event") == 0) {
		signal->type = ButtonRelease;
	} else {
		exit(0);
	}
	signal->callback = rb_block_proc();

	if (dock->signal == NULL) {
		dock->signal = signal;
	} else {
		tmp = dock->signal;
		while (tmp->next != NULL) {
			tmp = tmp->next;
		}
		tmp->next = signal;
	}
	signal->next = NULL;
//	obj = Data_Wrap_Struct(self, docksignal_mark, -1, signal);
}

static VALUE signal_check(struct WMDockSignal *signal, XEvent event)
{
	while (signal) {
		if (signal->type == event.type) {
			VALUE dockevent;
			printf ("hoge %d\n", event.type);
			signal->event = event;
			dockevent = dockevent_initialize(Qnil, event);
			rb_funcall(signal->callback, id_call, 1, dockevent);
			return Qtrue;
		}
		signal = signal->next;
	}
	return Qfalse;
}

static VALUE dockapp_signal_callback(WMDockApp *dock, XEvent event)
{
	VALUE ret;
	XEvent tmp_event = event;
	if (dock->signal) {
		struct WMDockSignal *signal;
		signal = dock->signal;
		ret = signal_check(signal, tmp_event);
		return ret;
	}
	return Qfalse;
}

static void signal_callback(WMDockItem *item, XEvent event)
{
	
	if (item->signal) {
		struct WMDockSignal *signal;
		signal = item->signal;
		while (signal) {
			if (signal->type == event.type) {
				VALUE dockevent;
				printf ("hoge %d\n", event.type);
				signal->event = event;
				dockevent = dockevent_initialize(Qnil, event);

				rb_funcall(signal->callback, id_call, 1, 
					   dockevent);

			}
			signal = signal->next;
		}
	}
#if 0
	if (item->callback) {
		/* TODO: to return as Hash */
		if (event.type == ButtonPress ||
		    event.type == ButtonRelease) {
			button_event = event.xbutton;
			rb_funcall(item->callback, id_call, 5,
				   INT2FIX(button_event.x),
				   INT2FIX(button_event.y),
				   INT2FIX(button_event.button),
				   INT2FIX(button_event.x_root),
				   INT2FIX(button_event.y_root));
		}
	}
#endif 
}

static VALUE dockapp_name(VALUE self)
{
        WMDockApp *dock;
        Data_Get_Struct(self, WMDockApp, dock);
        return rb_str_new2(dock->wname);
}

static void dockapp_delete(VALUE self, VALUE vitem)
{
	WMDockItem *item, *tmp;
	WMDockApp *dock;
	
	Data_Get_Struct(self, WMDockApp, dock);
	Data_Get_Struct(vitem, WMDockItem, item);

	item->visible = DOCKITEM_INVISIBLE;

	tmp = dock->item;
	init_pixmap(dock);
	while (tmp != NULL) {
		if (tmp->visible == DOCKITEM_VISIBLE) {
			set_pixmap(dock, tmp->x, tmp->y, 
				   tmp->x + tmp->width,
				   tmp->y + tmp->height);
		}
		tmp = tmp->next;
	}
	RedrawWindow(dock);
		
}

static WMDockItem* search_dockitem(WMDockApp *dock, WMDockItem *item)
{
	WMDockItem *tmp;
	tmp = dock->item;
	while (tmp != NULL) {
		if (tmp == item) {
			return tmp;
		}
		tmp = tmp->next;
	}
	return tmp;
}

static void dockapp_add(VALUE self, VALUE x, VALUE y, VALUE item)
{
	WMDockApp *dock;
	WMDockItem *dockitem;
	int margin = 4;

	Check_Type(x, T_FIXNUM);
	Check_Type(y, T_FIXNUM);

	Data_Get_Struct(self, WMDockApp, dock);
	Data_Get_Struct(item, WMDockItem, dockitem);

	if (dock->display == NULL) {
		rb_raise(rb_eRuntimeError, "not openwindow");
		return;
	}
	
	dockitem->dock = dock;
	dockitem->x = FIX2INT(x)+margin;
	dockitem->y = FIX2INT(y)+margin;

	if (dockitem->type != TYPE_POPUP) {
		if (dockitem->type == ITEMTYPE_RECTANGLE) {
			set_pixmap(dock, dockitem->x, dockitem->y, 
				   dockitem->x + dockitem->width,
				   dockitem->y + dockitem->height);
		} else {
			set_pixmap_circle(dock, dockitem->x, dockitem->y, 
					  dockitem->x + dockitem->width,
					  dockitem->y + dockitem->height);
		}
	}
	if (search_dockitem(dock, dockitem) == NULL) {
		if (dock->item == NULL) {
			dock->item = dockitem;
		} else {
			WMDockItem *ptr;
			ptr = dock->item;
			while (ptr->next != NULL) {
				ptr = ptr->next;
			}
			ptr->next = dockitem;
		}
		dockitem->next = NULL;
	}
	if (dockitem->type != TYPE_POPUP) {
		dockitem->visible = DOCKITEM_VISIBLE;
		AddMouseRegion(mouse_region_index,
			       dockitem->x, dockitem->y, 
			       dockitem->x + dockitem->width, 
			       dockitem->y + dockitem->height, 
			       dockitem);
		mouse_region_index++;
	}
}

/*
static void _dockapp_set_timer(VALUE self, VALUE interval)
{

	WMDockApp *dock;
	WMDockTimer *timer, *tmp;

	Check_Type(interval, T_FIXNUM);
	Data_Get_Struct(self, WMDockApp, dock);

	timer = malloc(sizeof(WMDockTimer));
	memset(timer, 0, sizeof(WMDockTimer));
	
	new_timer(timer, FIX2INT(interval)*1000);
	timer->callback = rb_block_proc();

	if (dock->timer == NULL) {
		dock->timer = timer;
	} else {
		tmp = dock->timer;
		while (tmp->next != NULL) {
			tmp = tmp->next;
		}
		tmp->next = timer;
	}
	timer_id += 1;
	//	Data_Wrap_Struct(self, NULL, NULL, timer);

	rbgobj_add_relative_removable(self, timer->callback,
				      id_relative_callbacks,
				      INT2FIX(timer_id));
}
*/

static void dockapp_openwindow(VALUE self)
{
	unsigned int	borderwidth = 1;
	XClassHint	classHint;
	char		*display_name = NULL;
	char		*wname;
	XTextProperty	name;
	XGCValues	gcv;
	unsigned long	gcm;
	int		dummy = 0;

	int pixmask_width = 64;
	int pixmask_height = 64;

	int		screen;
	int		d_depth;
	int x_fd;
	XSizeHints	mysizehints;
	XWMHints	mywmhints;
	Pixel		back_pix, fore_pix;
	char		*Geometry = "";
	char *loc;
	Pixmap pixmask;

	WMDockApp *wmdockapp;

	Data_Get_Struct(self, WMDockApp, wmdockapp);
	wname = strdup(wmdockapp->wname);

	/* taken from twm.c */
	loc = setlocale(LC_ALL, "");
	if (!loc || !strcmp(loc, "C") || !strcmp(loc, "POSIX") ||
	    !XSupportsLocale()) {
		wmdockapp->use_fontset = False;
	} else {
		wmdockapp->use_fontset = True;
	}

	if (!(display = XOpenDisplay(display_name))) {
		fprintf(stderr, "%s: can't open display %s\n",
			wname, XDisplayName(display_name));
		exit(1);
		/* TODO:raise exception */
	}
	wmdockapp->display = display;
	screen  = DefaultScreen(wmdockapp->display);
	Root = RootWindow(wmdockapp->display, screen);
	wmdockapp->Root    = Root;
	d_depth = DefaultDepth(wmdockapp->display, screen);
	x_fd    = XConnectionNumber(wmdockapp->display);


	/* Convert XPM to XImage */
	/* pixmap_bytes -> xpm_master */
	init_pixmap(wmdockapp);
	GetXPM(wmdockapp, &wmdockapp->wmgen, wmdockapp->xpm_master);

	GetXPM(wmdockapp, &wmdockapp->text_pixmap, text_xpm);

	GetXPM(wmdockapp, &wmdockapp->parts_pixmap, dockapp_parts_xpm);


	/* Create a window to hold the stuff */
	mysizehints.flags = USSize | USPosition;
	mysizehints.x = 0;
	mysizehints.y = 0;


	back_pix = GetColor(wmdockapp, "white");
	fore_pix = GetColor(wmdockapp, "black");


	XWMGeometry(wmdockapp->display, screen, Geometry, NULL, borderwidth,
		    &mysizehints, &mysizehints.x, &mysizehints.y,
		    &mysizehints.width,&mysizehints.height, &dummy);
	mysizehints.width = 64;
	mysizehints.height = 64;

	wmdockapp->win = XCreateSimpleWindow(wmdockapp->display, 
					     wmdockapp->Root, 
					     mysizehints.x, mysizehints.y,
					     mysizehints.width, 
					     mysizehints.height, 
					     borderwidth, fore_pix, back_pix);

	wmdockapp->iconwin = XCreateSimpleWindow(wmdockapp->display,
						 wmdockapp->win, 
						 mysizehints.x, mysizehints.y,
						 mysizehints.width, 
						 mysizehints.height, 
						 borderwidth, fore_pix, 
						 back_pix);

	/* Activate hints */
	XSetWMNormalHints(wmdockapp->display, wmdockapp->win, &mysizehints);
	classHint.res_name = wname;
	classHint.res_class = wname;
	XSetClassHint(wmdockapp->display, wmdockapp->win, &classHint);

	XSelectInput(wmdockapp->display, wmdockapp->win,
		     ButtonPressMask | ExposureMask | ButtonReleaseMask |
		     PointerMotionMask | StructureNotifyMask);
	XSelectInput(wmdockapp->display, wmdockapp->iconwin,
		     ButtonPressMask | ExposureMask | ButtonReleaseMask |
		     PointerMotionMask | StructureNotifyMask);

	if (XStringListToTextProperty(&wmdockapp->wname, 1, &name) == 0) {
		fprintf(stderr, "%s: can't allocate window name\n", 
			wmdockapp->wname);
		exit(1);
	}

	XSetWMName(wmdockapp->display, wmdockapp->win, &name);

	/* Create GC for drawing */
	gcm = GCForeground | GCBackground | GCGraphicsExposures;
	gcv.foreground = fore_pix;
	gcv.background = back_pix;
	gcv.graphics_exposures = 0;
	wmdockapp->NormalGC = XCreateGC(wmdockapp->display, wmdockapp->Root, 
			     gcm, &gcv);

	/* ONLYSHAPE ON */

	{
		static char mask_bits[64 * 64];

		createXBMfromXPM(mask_bits, wmdockapp->xpm_master, 64, 64);

		pixmask = XCreateBitmapFromData(wmdockapp->display,
						wmdockapp->win, 
						mask_bits, pixmask_width, 
						pixmask_height);

/*

	pixmask = XCreateBitmapFromData(wmdockapp->display, wmdockapp->win, xpm_mask_bits, pixmask_width, pixmask_height);
	XShapeCombineMask(wmdockapp->display, wmdockapp->win, ShapeBounding, 0, 0, pixmask, ShapeSet);
	XShapeCombineMask(wmdockapp->display, wmdockapp->iconwin, ShapeBounding, 0, 0, pixmask, ShapeSet);
*/

		XShapeCombineMask(wmdockapp->display, wmdockapp->win, 
				  ShapeBounding,
				  0, 0, pixmask, ShapeSet);
		XShapeCombineMask(wmdockapp->display, wmdockapp->iconwin, 
				  ShapeBounding, 0, 0, pixmask, ShapeSet);

	}

	/* ONLYSHAPE OFF */
	mywmhints.initial_state = WithdrawnState;
	mywmhints.icon_window = wmdockapp->iconwin;
	mywmhints.icon_x = mysizehints.x;
	mywmhints.icon_y = mysizehints.y;
	mywmhints.window_group = wmdockapp->win;
	mywmhints.flags =
		StateHint | IconWindowHint | IconPositionHint | WindowGroupHint;

	XSetWMHints(wmdockapp->display, wmdockapp->win, &mywmhints);

	XMapWindow(wmdockapp->display, wmdockapp->win);


	if (wmdockapp->use_fontset) {
		char **miss, *def;
		int n_miss; 

		wmdockapp->fontset = XCreateFontSet(wmdockapp->display, 
						    DEFAULT_FONT
						    , &miss, &n_miss, &def); 
		if (wmdockapp->fontset == NULL) {
			wmdockapp->fontset =
				XCreateFontSet(wmdockapp->display, 
					       SUBSTITUTE_FONT
					       , &miss, &n_miss, &def);
			printf ("using substitue font\n");
		}
	}
}

static void dockapp_start(VALUE self)
{
	XEvent	event;
	int s = 0;
	Display *display;
	WMDockApp *dock;
	WMDockTimer *timer;

	Data_Get_Struct(self, WMDockApp, dock);
	
	if (dock->display == NULL) {
	  rb_raise(rb_eRuntimeError, "not openwindow");
	  return;
	}
	display = dock->display;

	/* Loop Forever */
	while (1) {
		/* timer function */
		timer = dock->timer;
		while (timer != NULL) {
			update_timer(timer);
			timer = timer->next;
		}
		timer = docktimer;
		while (timer != NULL) {
			update_timer(timer);
			timer = timer->next;
		}
/*
		item = dock->item;
		while (item != NULL) {
			if (item->timer != NULL) {
				update_timer(item->timer);
			}
			item = item->next;
		}
*/
		while (XPending(display)) {
			XNextEvent(display, &event);
			switch (event.type) {
			case Expose:
				RedrawWindow(dock);
				/*				printf ("Expose\n");*/


				break;
                        case DestroyNotify:
                                XCloseDisplay(display);
				printf ("destroy\n");
                                //exit(EXIT_SUCCESS);
                                break;
			case ButtonPress:
				/* 1st: check signal of DockApp */
				dockapp_signal_callback(dock, event);
				s = CheckMouseRegion(event.xbutton.x,
						     event.xbutton.y);
				printf ("ButtonPress: %d(%d, %d)\n", s,
					event.xbutton.x, event.xbutton.y);
				if (s >= 0 && mouse_region[s].item != NULL) {
					signal_callback(mouse_region[s].item, 
							event);
				}
				break;
			case ButtonRelease:
				/* 1st: check signal of DockApp */
				if (dockapp_signal_callback(dock, event) == Qtrue) {
					break;
				}
				printf ("ButtonRelease: %d(%d, %d)\n", s,
					event.xbutton.x, event.xbutton.y);
				signal_callback(mouse_region[s].item, event);
				break;
				
			}
		}
		usleep(10000); /* 10ms */
	}
}

static void dockapp_destroy(VALUE self)
{
	Display *display;
	WMDockApp *dock;
	Data_Get_Struct(self, WMDockApp, dock);
	
	if (dock->display == NULL) {
	  rb_raise(rb_eRuntimeError, "not openwindow");
	  return;
	}
	display = dock->display;
	XCloseDisplay(display);
	exit(0);
}

static VALUE dockapp_s_new(int argc, VALUE *argv, VALUE self)
{
	VALUE obj;
	WMDockApp *dock;
	VALUE vname;

	dock = malloc(sizeof(WMDockApp));
	memset(dock, 0, sizeof(*dock));

        if (rb_scan_args(argc, argv, "01", &vname) == 0) {
          dock->wname = strdup("WMDockApp");
        } else {
        Check_Type(vname, T_STRING);
          dock->wname = strdup(StringValuePtr(vname));
        }

	obj = Data_Wrap_Struct(self, dockapp_mark, -1, dock);

	signal(SIGINT, sig_int);

	return obj;
}

void Init_dockapp(void) {
	VALUE rb_DockApp;

	id_call = rb_intern("call");
	mDockApp = rb_define_module("Dock");
	rb_ivar_set(mDockApp, id_relative_callbacks, Qnil);
	signal(SIGINT, sig_int);
	memset(mouse_region, 0, sizeof(MOUSE_REGION)*MAX_MOUSE_REGION);
	mouse_region_index = 0;

	rb_DockApp = rb_define_class("DockApp", rb_cObject);
	rb_define_singleton_method(rb_DockApp, "new", dockapp_s_new, -1);
	rb_define_method(rb_DockApp, "openwindow",
			 RUBY_METHOD_FUNC(dockapp_openwindow), 0);
	rb_define_method(rb_DockApp, "start", 
			 RUBY_METHOD_FUNC(dockapp_start), 0);
	rb_define_method(rb_DockApp, "destroy", 
			 RUBY_METHOD_FUNC(dockapp_destroy), 0);
	rb_define_method(rb_DockApp, "add", 
			 RUBY_METHOD_FUNC(dockapp_add), 3);
	rb_define_method(rb_DockApp, "delete", 
			 RUBY_METHOD_FUNC(dockapp_delete), 1);
//	rb_define_method(rb_DockApp, "set_timer", 
//			 RUBY_METHOD_FUNC(dockapp_set_timer), 1);

	rb_define_method(rb_DockApp, "signal_connect",
			 RUBY_METHOD_FUNC(dockapp_signal_connect), 1);
        rb_define_method(rb_DockApp, "name",
                         RUBY_METHOD_FUNC(dockapp_name), 0);

	dockitem_init(rb_DockApp);
	docktext_init(rb_DockApp);
	docktimer_init(rb_DockApp);
	dockpopup_init(rb_DockApp);
	dockevent_init(rb_DockApp);
#if 0
	gtk_dockapp_init();
#endif

}
