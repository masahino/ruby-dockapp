#ifndef DOCKITEM_H_INCLUDED
#define DOCKITEM_H_INCLUDED

#define LED_TYPE_CIRCLE 1
#define LED_TYPE_SQUARE 2

#define LED_COLOR_GREEN  1
#define LED_COLOR_RED    2
#define LED_COLOR_YELLOW 3
#define LED_COLOR_OFF    4

/* dockitem.c */
void dockitem_callback(VALUE self);
void dockitem_mark(WMDockItem *item);
void dockitem_init(VALUE rb_DockApp);
void dockitem_signal_connect(VALUE self, VALUE signal_type);
void dockitem_settip(VALUE self, VALUE text);
void dockitem_show_tooltips(WMDockItem *item);
void dockitem_hide_tooltips(WMDockApp *dock);

#endif
