require '../dockapp.so'
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
        p event.time
        #                menu.popup(nil, nil, event.button, event.time)
        #        menu.popup(nil, nil, event.button, Time.now.usec)
        menu.popup(nil, nil, event.button, Gdk::Event::CURRENT_TIME)
  end
  window.set_default_size(20, 10)
  return [window, menu]
end


Gtk.init

menutest = DockApp.new("menutest")
menutest.openwindow
item = DockApp::Item.new(20, 20)

menutest.add(10, 10, item)
(window, popup) = make_popup2

item.click_callback do |x, y, button, x_root, y_root|
  window.move(x_root, y_root).show_all
  p popup
#  popup.popup(nil, nil, 0, Gdk::Event::CURRENT_TIME)
end

timer = DockApp::Timer.new(100) do
  while (Gtk.events_pending?)
         Gtk.main_iteration
       end
end

timer.start

menutest.start


