# Author: HINO Masayuki(masahino@tky3.3web.ne.jp)
# 
# 
# 

class DockApp
  def initialize(window_name)
  end

  # �����Ƥ֤�window��ɽ�������
  def openwindow()
  end
  
  def destroy()
  end

  # item�ǻ��ꤷ��DockItem��x, y����Ͽ����
  def add(x, y, dockitem)
  end

  # item�ǻ��ꤷ��DockItem��ä�
  def add(dockitem)
  end
  
  # ���٥���Ԥ��Υ롼��
  def start()
  end


  # Dock window��Υ��֥�����ɥ�
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

  # Dock���ץꥱ�������ˤ褯����LED���ʥƥ����Ȥ�ɽ�����뤿��Υ��ꥢ
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
