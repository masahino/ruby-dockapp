require './dockapp'
require 'gtk2'

def make_popup
  window = Gtk::Window.new(Gtk::Window::POPUP)
  window.show_all
  return window
end

def make_popup2
window = Gtk::Window.new(Gtk::Window::POPUP)
menu = Gtk::Menu.new
menu.append(Gtk::MenuItem.new("Test1"))
menu.append(Gtk::MenuItem.new("Test2"))

menu.show_all
window.add_events(Gdk::Event::BUTTON_PRESS_MASK)
window.signal_connect("button_press_event") do |widget, event|
	if event.kind_of? Gdk::EventButton
          if (event.button == 3) 
            menu.popup(nil, nil, event.button, event.time)
          end
	end
  end
window.set_default_size(10, 10).show_all
return [window, menu]
end
  Gtk.init

menutest = DockApp.new("menutest")
menutest.openwindow
item = DockApp::Item.new(20, 20)

menutest.add(10, 10, item)

item.click_callback do |x, y, button, x_root, y_root|
  (window, popup) = make_popup2
  window.move(x_root, y_root)
  popup.reposition
  popup.popup(nil, nil, button, 0) do
    puts "popup"
  end
#  Gtk.main_iteration_do(false)
  Gtk.main
end

menutest.start


