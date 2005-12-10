require './dockapp.so'

dock = DockApp.new("tooltips")
dock.openwindow
item1 = DockApp::Item.new(20, 20)
item2 = DockApp::Item.new(20, 20)

dock.add(0, 0, item1)
dock.add(30, 30, item2)

item1.set_tip("this is item1")

item2.set_tip("this is item2")

dock.start
