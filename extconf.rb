require 'mkmf'

if have_header("X11/Xlib.h") and find_library('X11', 'XOpenDisplay', '/usr/X11R6/lib') and find_library('Xpm', 'XpmCreatePixmapFromData', '/usr/X11R6/lib')  and find_library('Xext', 'XShapeCombineMask', '/usr/X11R6/lib')
	create_makefile('dockapp')
end

make = open("Makefile", "a")
make.print <<-EOF

rdoc: doc/index.html

doc/index.html: doc/dockapp.rb
\t@-(rdoc -c euc-jp doc/dockapp.rb)

EOF

