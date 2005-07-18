#!/usr/bin/env ruby

require 'dockapp'

class PopUpTest
  def initialize
    @dock = DockApp.new("PopUpTest")
    @dock.openwindow
    
    @popup1 = DockApp::PopUp.new(1, 1)
    @item1 = DockApp::Item.new(20, 12)
    @item2 = DockApp::Item.new(30, 12)

    @dock.add(10, 10, @popup1)
    @dock.add(10, 10, @item1)
    @dock.add(10, 30, @item2)
    @item1.draw_string(1, 10, "UP")
    @item2.draw_string(1, 10, "DOWN")
    @popup1.add_item("test1\ntest2\ntest3")

    self.set_action
  end

  def set_action
    @item1.signal_connect("button_press_event") do |event|
      if event.button == 1
        @popup1.show(0, 0, DockApp::PopUp::UP)
      elsif event.button == 3
        @popup1.popup(0) do |n|
          puts n
        end
      end
    end

    @item2.signal_connect("button_press_event") do |event|
      @popup1.show(0, 0, DockApp::PopUp::DOWN)
    end
    
    @item1.signal_connect("button_release_event") do |event|
      @popup1.hide
    end
    @item2.signal_connect("button_release_event") do |event|
      @popup1.hide
    end

  end

  def start
    @dock.start
  end
end




# test
popuptest = PopUpTest.new
popuptest.start
