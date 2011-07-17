require 'uim'
require 'dockapp'
require 'iconv'

$KCODE='e'

class WmUIMHelper
  def initialize
    @uim_helper = UIM::Helper.new
    @uim_helper.init_client
    @uim_helper.send_message("im_list_get\n");
    @uim_helper.get_prop_list

    @dockapp = DockApp.new("WMUIM")
    @dockapp.openwindow
    @label = Array.new
    @label = DockApp::Item.new(55, 15)
    @description = DockApp::Item.new(55, 16)
    @imname = DockApp::Text.new("", 6, 1, 0)
    @input = DockApp::Item.new(14, 14)
    @dockapp.add(0, 0, @label)
    @dockapp.add(0, 20, @description)
    @dockapp.add(15, 40, @imname)
    @dockapp.add(0, 40, @input)
    
    @prop_list = Array.new
    @im_list = Array.new

    set_timer
    set_button_callback
  end

  def set_button_callback
    @label.click_callback do |x, y|
      pos = 0
      @prop_list.each do |p|
        list = p.split("\t")
        text = list[1]
        pos += text.length*6
          puts "pos = "+pos.to_s
        if x < pos
          @uim_helper.send_message("prop_activate\n"+list[4]+"\n")
          break
        end
      end
    end
    
    @description.click_callback do |x, y|
      for i in 0..@prop_list.size-1
        if @prop_list[i].split("\t")[5] == "*"
          n = i+1
          if n > @prop_list.size-1
            n = 0
          end
          @uim_helper.send_message("prop_activate\n"+@prop_list[n].split("\t")[4]+"\n")
          break
        end
      end
    end
    
    @imname.click_callback do
      system("uim-im-switcher")
    end
  end

  def set_timer
    label_timer = DockApp::Timer.new(100) do 
      if @uim_helper.readable
        @uim_helper.read_proc
        loop do
          str =  @uim_helper.get_message
          if str != nil
            message_array = str.split("\n")
            check_message(message_array)
          else
            break
          end
        end   
      end
    end
    label_timer.start
  end

  def set_label(label, description)
    @label.clear
    x = 2
    for i in 0..@prop_list.size-1
      text = @prop_list[i].split("\t")[1]
      if text == label
        @label.draw_string(x, 12, text)
      else
        @label.draw_string(x, 12, text, "#000049244103")
      end
      x += text.length*6
    end
    @description.clear
    @description.draw_string(2, 12, description[0..8])
  end

  def parse_message(message_array)
    charset = message_array.shift
    dummy, charset = charset.split("=")
    return charset, message_array
  end    

  def im_list_update(message_array)
    charset, message_array = parse_message(message_array)
    @im_list = message_array
    @im_list.each do |im|
      if im.split("\t")[3] =~ /selected/
        @imname.set_text(im.split("\t")[0])
        break
      end
    end
  end
  
  def prop_label_update(message_array)
    charset, message_array = parse_message(message_array)
    label = message_array.shift
    label = Iconv.conv("euc-jp", charset, label)
    label, description = label.split("\t")
    set_label(label, description)
  end

  def prop_list_update(message_array)
    charset, message_array = parse_message(message_array)
    branch = message_array.shift
    branch = Iconv.conv("euc-jp", charset, branch)
    dummy, label, description = branch.split("\t")
    message_array.map! {|str| str = Iconv.conv("euc-jp", charset, str)}
    @prop_list = message_array
    set_label(label, description)
  end

  def check_message(message_array)
    if message_array[0] =~ /prop_label_update/
      prop_label_update(message_array[1..-1])
    elsif message_array[0] =~ /prop_list_update/
      prop_list_update(message_array[1..-1])
    elsif message_array[0] == "im_list"
      im_list_update(message_array[1..-1])
    else
      puts message_array[0]
    end
  end

  def start
    @dockapp.start
  end
end

wmuim = WmUIMHelper.new
wmuim.start
