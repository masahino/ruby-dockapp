require 'dockapp'

class DockSample

  def initialize 
    @dockapp = DockApp.new("TestApp") # name
    @time = DockItem.new(55, 12) # default string, width, height
    @system_info = DockItem.new(55, 24) 
    @texttest = DockText.new("", 8, 2, 0) # default string, columns, rows

    @dockapp.openwindow 
    @dockapp.add_item(@time, 0, 5) # item, x, y
 
    @graphtest = DockItem.new(55, 36)
#    @dockapp.add_item(@graphtest, 5, 20)

    @dockapp.add_item(@system_info, 0, 20)

#    @dockapp.add_item(@texttest, 10, 36)
    @texttest.set_text("test")

    @time.click_callback do
      puts "clicked"
    end

    @time.set_timer(1) do
      date = `date '+%H:%M:%S'`
      @time.drawLEDtext(date, 2, 2, 0) # x, y, string
    end

    @system_info.set_timer(10) do
      data = get_df_value()
      @system_info.drawLEDtext(data, 2, 2, 0)
      data = get_loadavg()
      @system_info.drawLEDtext(data, 2, 12, 0)
    end
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
