#ifndef DOCKAPP_EVENT_H
#define DOCKAPP_EVENT_H

#include "dockapp.h"


int get_Xsignal_type(char *signal_type);
int wait_Xevent(WMDockApp *dock, int event_type);

#endif /* DOCKAPP_EVENT_H */
