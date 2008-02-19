require '../dockapp.so'

dock = DockApp.new("itemtype")
dock.openwindow
item = DockApp::Item.new(25, 25, DockApp::Item::Circle)
box = DockApp::Item.new(30, 24)
dock.add(0, 0, item)
dock.add(25, 0, box)
draw_test = DockApp::Item.new(55, 10)
dock.add(0, 26, draw_test)
mbtest = DockApp::Item.new(55, 16)

dock.add(0, 40, mbtest)

mbtest.draw_string(2, 12, "おはよう")

box.drawLEDstring(2, 2, "test", 0)
box.draw_string(2, 20, "test", "red")


draw_test.draw_led( 5, 4, "green")
draw_test.draw_led(15, 4, "red")
draw_test.draw_led(25, 4, "yellow")
draw_test.draw_led(35, 4, "off")


dock.start
