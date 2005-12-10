require './dockapp.so'

dock = DockApp.new("itemtype")
dock.openwindow
item = DockApp::Item.new(20, 20, "circle")

box = DockApp::Item.new(20, 20)
dock.add(0, 0, item)
dock.add(30, 30, box)

dock.start
