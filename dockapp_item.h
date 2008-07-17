#ifndef DOCKAPP_ITEM_H
#define DOCKAPP_ITEM_H

#define LED_TYPE_CIRCLE 1
#define LED_TYPE_SQUARE 2

#define LED_COLOR_GREEN  1
#define LED_COLOR_RED    2
#define LED_COLOR_YELLOW 3
#define LED_COLOR_OFF    4

/* dockapp_item.c */
VALUE dockitem_callback(VALUE self);
void dockitem_mark(WMDockItem *item);
void dockitem_init(VALUE rb_DockApp);
VALUE dockitem_signal_connect(VALUE self, VALUE signal_type);
VALUE dockitem_settip(VALUE self, VALUE text);
VALUE dockitem_show_tooltips(WMDockItem *item);
VALUE dockitem_hide_tooltips(WMDockApp *dock);

#endif /* DOCKAPP_ITEM_H */
