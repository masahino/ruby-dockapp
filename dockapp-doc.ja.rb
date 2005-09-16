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

  # window��ä�
  def destroy()
  end

  # dockitem�ǻ��ꤷ��DockItem��Dock��x, y����Ͽ����
  def add(x, y, dockitem)
  end

  # dockitem�ǻ��ꤷ��DockItem��ä�
  def delete(dockitem)
  end
  
  # ���٥���Ԥ��Υᥤ��롼��
  def start()
  end


  # Dock window��Υ��֥�����ɥ�
  class Item
    def initialize(width, height)
    end

    # x, y�ΰ��֤�string�ǻ��ꤷ��ʸ�����ɽ������
    def draw_string(x, y, string)
    end

    # LED����ʸ�����ɽ�����롣 color��ʸ���ο�����ꤹ�롣
    # 0 = , 1 = 
    def drawLEDstring(x, y, string, color = 0)
    end

    # obsolute
    def click_callback() # :yields: x, y
    end

    # Item�򥯥ꥢ
    def clear()
    end

    # pixmap�ե�����̾����ꤷ�Ʋ�����ɽ�����롣
    def set_pixmap(pixmap_filename)
    end

    # ��������褷���ɤ��٤���
    def draw_rect(x, y, width, height, color)
    end

    # ��������
    def draw_line(x1, y1, x2, y2, color)
    end

    # ��������
    def draw_point(x, y, color)
    end

  end

  # Dock���ץꥱ�������ˤ褯����LED���ʥƥ����Ȥ�ɽ�����뤿��Υ��ꥢ
  class Text
    def initialize(text, width, height, color = 0)
    end

    # text�ǻ��ꤷ��ʸ�����ɽ��
    def set_text(text, color = 0)
    end
    
    # obsolute
    def click_callback() # :yields: x, y
    end

  end

  # Timer���饹
  class Timer
    # ñ�̤�ms
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
