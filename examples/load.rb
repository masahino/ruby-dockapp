require '../dockapp'

class Load

  def initialize 
    @dockapp = DockApp.new("Load") # name
    @dockapp.openwindow 

    width = 53
    height = 53
    offset = 1
    load_item = DockApp::Item.new(width+2, height+2)
    
    load_data = Array.new(width)
    @dockapp.add(0, 0, load_item)
    @t1 = DockApp::Timer.new(10000) do
      tmp = `cat /proc/loadavg`
      a = tmp.split(/\s+/)
      load_data.push(a[0].to_f)
      max = get_max(load_data).to_i
      load_data.shift
      load_item.clear

      for i in 0..width-1
        if load_data[i] == nil
          next
        end
        load_item.draw_line(i + offset , 
                            (height-((height*load_data[i]).to_i/(max+1))) +
                              offset, 
                            i + offset , height + offset,
                            "blue") # TODO: change color
      end

      if max >= 1
        for i in 1..max
          load_item.draw_line(offset, i*(height/(max+1)) + offset, 
                              width, i*(height/(max+1)) + offset,
                              "red")
        end
      end
    end

  end
  def get_max(data)
    max = 0
    data.each do |d|
      if d == nil 
        next
      end
      if d > max 
        max = d
      end
    end
    return max
  end

  def run
    @t1.start
    @dockapp.start
  end
end

load = Load.new()
load.run
