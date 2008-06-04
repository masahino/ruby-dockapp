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

#define WMDOCKTIMER_STOP  0
#define WMDOCKTIMER_START 1

WMDockTimer *docktimer;
extern ID id_call;

static void docktimer_mark(WMDockTimer *timer)
{
	rb_gc_mark(timer->callback);
}

void new_timer(WMDockTimer *timer, int interval)
{
	timerclear(&timer->interval);
	if (interval >= 1000000) {
		timer->interval.tv_sec = interval/1000000;
		interval %= 1000000;
	}
	timer->interval.tv_usec = interval;
	gettimeofday(&timer->next_time, NULL);
	timeradd(&timer->next_time, &timer->interval, &timer->next_time);
}

void update_timer(WMDockTimer *timer)
{
	struct timeval current;
	if (timer->status == WMDOCKTIMER_STOP) {
		return;
	}
	//	printf ("interval = %u\n", timer->interval.tv_sec);
	//	printf ("next_time = %u\n", timer->next_time.tv_sec);

	gettimeofday(&current, NULL);
	//	printf ("current = %u\n", current.tv_sec);
	if (timercmp(&current, &timer->next_time, >)) {
		timeradd(&current, &timer->interval, &timer->next_time);
		//		printf ("rb_funcall\n");
		rb_funcall(timer->callback, id_call, 0);
	}
}

static void docktimer_start(VALUE self)
{
	WMDockTimer *timer;

	Data_Get_Struct(self, WMDockTimer, timer);
	timer->status = WMDOCKTIMER_START;
	update_timer(timer);
}

static VALUE docktimer_getstatus(VALUE self)
{
	WMDockTimer *timer;

	Data_Get_Struct(self, WMDockTimer, timer);
	return INT2FIX(timer->status);
}


static void docktimer_stop(VALUE self)
{
	WMDockTimer *timer;

	Data_Get_Struct(self, WMDockTimer, timer);
	timer->status = WMDOCKTIMER_STOP;
}

static VALUE docktimer_initialize(VALUE self, VALUE interval)
{
	VALUE obj;

	WMDockTimer *timer, *tmp;

	Check_Type(interval, T_FIXNUM);

	timer = malloc(sizeof(WMDockTimer));
	memset(timer, 0, sizeof(WMDockTimer));

	
	new_timer(timer, FIX2INT(interval)*1000);
	timer->callback = rb_block_proc();
	timer->status = WMDOCKTIMER_STOP;

	if (docktimer == NULL) {
		docktimer = timer;
	} else {
		tmp = docktimer;
		while (tmp->next != NULL) {
			tmp = tmp->next;
		}
		tmp->next = timer;
	}

	obj = Data_Wrap_Struct(self, docktimer_mark, -1, timer);
	return obj;
}

void docktimer_init(VALUE rb_DockApp)
{
	VALUE rb_DockTimer;

	rb_DockTimer = rb_define_class_under(rb_DockApp, "Timer", rb_cObject);
	rb_define_singleton_method(rb_DockTimer, "new", docktimer_initialize, 1);
	rb_define_method(rb_DockTimer, "start", RUBY_METHOD_FUNC(docktimer_start), 0);
	rb_define_method(rb_DockTimer, "stop", RUBY_METHOD_FUNC(docktimer_stop), 0);
	rb_define_method(rb_DockTimer, "get_status", RUBY_METHOD_FUNC(docktimer_getstatus), 0);
}
