#!/usr/bin/env ruby

require 'dockapp'

class KBannerTest
  def initialize
    @dock = DockApp.new("KBannerTest")
    @dock.openwindow
    @item = DockApp::Item.new(50, 50)

    @dock.add(2, 2, @item)
      test_kbanner("ふがふが", "s")
  end

  def test_kbanner(str, s = nil)
    if s != nil
      option = "-s"
    end
    kt = `echo -n \"#{str}\" | kbanner -l #{option}`
    kstr = kt.split("\n")
    maxx = kstr.size
    maxy = kstr[0].size
    for y in 0..maxy-1
      for x in 0..maxx-1
        if  kstr[x][maxy-1-y, 1] != " "
          @item.draw_point(x, y, "#2081B2CAAEBA")
        else 
          @item.draw_point(x, y, "#208120812081")
        end
      end
    end
  end

  def run
    @dock.start
  end
end



kbanner = KBannerTest.new
kbanner.run
