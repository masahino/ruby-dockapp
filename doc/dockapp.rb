# Author: HINO Masayuki(masahino@tky3.3web.ne.jp)
# 
# 
# 

class DockApp
  def initialize(window_name)
  end

  # これを呼ぶとwindowが表示される
  def openwindow()
  end
  
  def destroy()
  end

  # itemで指定したDockItemをx, yに登録する
  def add(x, y, dockitem)
  end

  # itemで指定したDockItemを消す
  def add(dockitem)
  end
  
  # イベント待ちのループ
  def start()
  end


  # Dock window内のサブウィンドウ
  class Item
    def initialize(width, height)
    end

    def draw_string(x, y, string)
    end

    def drawLEDstring(x, y, string, color = 0)
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

  # DockアプリケーションによくあるLED風なテキストを表示するためのエリア
  class Text
    def initialize(text, width, height, color = 0)
    end

    def set_text(text, color = 0)
    end
    
    def click_callback() # :yields: x, y
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
end
