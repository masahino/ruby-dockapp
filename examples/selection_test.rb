#!/usr/bin/env ruby 

require './dockapp.so'
require 'xselection'

class SelectionTest
  def initialize
    @dock = DockApp.new("SelectionTest")
    @dock.openwindow

    stack = Array.new(10)

    @area = DockApp::Item.new(55, 55)
    @dock.add(0, 0, @area)
    xs = Xselection.new(":1.0")
    t1 = DockApp::Timer.new(100) do
      if xs.check != nil
        stack.push(xs.get)
        str = stack.last
        @area.clear
        n = 1
        str.split("\n").each do |line|
          @area.draw_string(1, 10*n, line)
          n += 1
          if n > 5
            break
          end
        end
      end
      if stack.size > 10
        stack.shift
      end
    end
    t1.start

#    @area.signal_connect("selection_notify_event") do |e|
#      puts e
#    end
  end
  
  def start
    @dock.start
  end
end

st = SelectionTest.new
st.start
