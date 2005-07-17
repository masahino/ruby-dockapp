# Author: HINO Masayuki(masahino@tky3.3web.ne.jp)
# 
# 
# 

class DockApp
  def initialize(window_name="WMDockApp")
  end

  # show window
  def openwindow()
  end
  
  def destroy()
  end

  # regist item at x, y
  def add(x, y, dockitem)
  end

  # delete dockitem
  def delete(dockitem)
  end
  
  # event loop
  def start()
  end

  def signal_connect(signal) # :yields:event
  end
  
  def name()
  end

  # subwindow
  class Item
    def initialize(width, height)
    end

    def draw_string(x, y, string)
    end

    def drawLEDstring(x, y, string, color = 0)
    end

    def set_pixmap(pixmap_file)
    end

    def draw_point(x, y, color)
    end
    
    def draw_line(x1, y1, x2, y2, color)
    end

    def draw_rect(x1, y1, x2, y2, color)
    end

    def draw_led(x, y, color, type="circle", width=nil, height=nil)
    end
    
    def width()
    end

    def height()
    end

    def signal_connect(signal_name) # :yields:event
    end
    
    def click_callback() # :yields: x, y
    end

    def clear()
    end

    def set_pixmap(pixmap_filename)
    end

    def draw_rect(x, y, width, height, color)
    end

    def draw_line(x1, y1, x2, y2, color)
    end

    def draw_point(x, y, color)
    end

  end

  # 
  class Text
    def initialize(text, width, height, color = 0)
    end

    def set_text(text, color = 0)
    end
    
    def click_callback() # :yields: x, y
    end

    def signal_connect(signal_name) # :yields:event
    end

  end

  class Timer
    def initialize(interval) # :yields: 
    end

    # start timer
    def start() 
    end

    # stop timer
    def stop()
    end

    # get timer status
    def get_status()
    end

  end

  # popup window
  class PopUp
    def initialize(width, height)
    end

    # show popup window at (x, y)
    def show(x, y)
    end

    # hide popup window
    def hide
    end

    # NOT implement
    def popup
    end

    def show(x, y)
    end
    
    def hide()
    end

    def get_index()
    end

    def add_item(item_list)
    end

  end

  # popup window with a XPM image
  class PopUpImage < PopUp

    def initialize(xpm_filename)
    end

    def width()
    end

    def height()
    end

  end

  
end
