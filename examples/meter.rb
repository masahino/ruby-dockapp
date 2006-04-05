require '../dockapp'
dock = DockApp.new("meter")

meter = DockApp::Item.new(50, 10)

dock.add(5, 20, meter)

# 0から49は0xff0000 -> 0xffff00まで、Gを増やす
# 50から99は 0xffff00 -> 0x00ff00まで、Rを減らす
# 連続か? 段階か?

# 連続
# 幅50とする。
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
