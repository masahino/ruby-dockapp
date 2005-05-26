require '../dockapp'

class DockSample

  def initialize 
    @dockapp = DockApp.new("TestApp") # name
    @time = DockApp::Item.new(55, 12) # default string, width, height
    @system_info = DockApp::Item.new(55, 24) 
    @texttest = DockApp::Text.new("", 8, 2, 0) # default string, columns, rows

    @dockapp.openwindow 
    @dockapp.add(0, 5, @time) # item, x, y
 
    @graphtest = DockApp::Item.new(55, 36)
#    @dockapp.add_item(@graphtest, 5, 20)

    @dockapp.add(0, 20, @system_info)

#    @dockapp.add_item(@texttest, 10, 36)
    @texttest.set_text("test")

    @time.click_callback do
      puts "clicked"
    end

    @time_timer = DockApp::Timer.new(1000) do
      date = `date '+%H:%M:%S'`
      @time.drawLEDstring(2, 2, date, 0) # x, y, string
    end
    @time_timer.start

    @system_timer = DockApp::Timer.new(10000) do
      data = get_df_value()
      @system_info.drawLEDstring(2, 2, data, 0)
      data = get_loadavg()
      @system_info.drawLEDstring(2, 12, data, 0)
    end
    @system_timer.start
    
  end

  def get_df_value
    tmp_array = `df | grep hda2`.chomp.split(/\s+/)
    return sprintf "%s %s", tmp_array[5], tmp_array[4]
  end
  
  def get_loadavg
    tmp_array = `cat /proc/loadavg`.chomp.split(/\s+/)
    return sprintf "L %s", tmp_array[0]
  end

  def run
    @dockapp.start
  end

end

sample = DockSample.new
sample.run
