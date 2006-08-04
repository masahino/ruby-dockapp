#ifndef DOCKAPP_TOOLTIPS_H
#define DOCKAPP_TOOLTIPS_H

#include "dockapp.h"

void hide_tooltip_window(Display *display, Window win);
Window create_tooltip_window(WMDockApp *dock);
void update_tooltip_window(WMDockApp *dock, Window win,
			   int x, int y, char *text);

#endif /* DOCKAPP_TOOLTIPS_H */
