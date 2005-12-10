require './dockapp'

dock = DockApp.new("LED test")
dock.openwindow
item = DockApp::Item.new(40, 40)
dock.add(10, 10, item)

item.draw_led(10, 10, "green")
item.draw_led(15, 10, "red")
item.draw_led(20, 10, "yellow")
item.draw_led(10, 15, "off")

item.draw_rect(10, 10, 10, 10, "green")
item.draw_line(10, 10, 20, 10, "white")
item.draw_line(10, 10, 10, 20, "white")

dock.start
