require '../dockapp'
dock = DockApp.new("meter")

meter = DockApp::Item.new(50, 10)

dock.add(5, 20, meter)

# 0����49��0xff0000 -> 0xffff00�ޤǡ�G�����䤹
# 50����99�� 0xffff00 -> 0x00ff00�ޤǡ�R�򸺤餹
# Ϣ³��? �ʳ���?

# Ϣ³
# ��50�Ȥ��롣
# 
p = 100
  width = 50
  max = width*p/100
  step = 256/(width/2)
  r = 0xff
  g = 0
  for i in 0..max-1
    if i < width/2
      g = i*step
    else
      r = 255-(i-width/2)*step
      g = 0xff
    end
    color = sprintf "#%02x%02x00", r, g
    meter.draw_line(i, 0, i, 9, color)
  end


dock.start
