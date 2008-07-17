require 'mkmf'

#dir_config('gtk', '/usr/include/gtk-2.0', '/usr/lib')
#have_header('gtk/gtk.h')
#dir_config('glib', '/usr/include/glib-2.0', '/usr/lib')
#dir_config('glibconfig', '/usr/lib/glib-2.0')
#dir_config('pango', '/usr/include/pango-1.0', '/usr/lib')
#dir_config('gdkconfig', '/usr/lib/gtk-2.0')
#dir_config('atk', '/usr/include/atk-1.0/', '/usr/lib')
#find_library('gtk-x11-2.0', 'gtk_init')
#find_library('gdk-x11-2.0', 'gdk_rgb_init')

have_header('Imlib2.h')
find_library('Imlib2', 'imlib_context_new')

have_header('wraster.h')
find_library('wraster', 'RSupportedFileFormats')



if have_header("X11/Xlib.h") and find_library('X11', 'XOpenDisplay', '/usr/X11R6/lib') and find_library('Xpm', 'XpmCreatePixmapFromData', '/usr/X11R6/lib')  and find_library('Xext', 'XShapeCombineMask', '/usr/X11R6/lib')
  create_header()
  create_makefile('dockapp')
end


make = open("Makefile", "a")
make.print <<-EOF

rdoc:
\t@-(rdoc -dU *.c)

EOF

