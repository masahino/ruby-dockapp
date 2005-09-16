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

  # windowを消す
  def destroy()
  end

  # dockitemで指定したDockItemをDockのx, yに登録する
  def add(x, y, dockitem)
  end

  # dockitemで指定したDockItemを消す
  def delete(dockitem)
  end
  
  # イベント待ちのメインループ
  def start()
  end


  # Dock window内のサブウィンドウ
  class Item
    def initialize(width, height)
    end

    # x, yの位置にstringで指定した文字列を表示する
    def draw_string(x, y, string)
    end

    # LED風の文字列を表示する。 colorは文字の色を指定する。
    # 0 = , 1 = 
    def drawLEDstring(x, y, string, color = 0)
    end

    # obsolute
    def click_callback() # :yields: x, y
    end

    # Itemをクリア
    def clear()
    end

    # pixmapファイル名を指定して画像を表示する。
    def set_pixmap(pixmap_filename)
    end

    # 矩形を描画して塗り潰す。
    def draw_rect(x, y, width, height, color)
    end

    # 線を描画
    def draw_line(x1, y1, x2, y2, color)
    end

    # 点を描画
    def draw_point(x, y, color)
    end

  end

  # DockアプリケーションによくあるLED風なテキストを表示するためのエリア
  class Text
    def initialize(text, width, height, color = 0)
    end

    # textで指定した文字列を表示
    def set_text(text, color = 0)
    end
    
    # obsolute
    def click_callback() # :yields: x, y
    end

  end

  # Timerクラス
  class Timer
    # 単位はms
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

    # 
    def popup  # :yields: index
    end
  end

  # popup window with a XPM image
  class PopUpImage < PopUp

    def initialize(xpm_filename)
    end
    
  end

  
end
