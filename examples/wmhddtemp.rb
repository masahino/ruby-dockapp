#!/usr/bin/env ruby

require 'dockapp'
require 'socket'

host_list = ["panda", "bear", "tiger"]

class HddTemp
  attr_accessor :host, :temp

  def initialize(host, port = 7634)
    @host = host
    @port = port
    @temp = []
  end
  def get()
    @temp = []
    begin
      s =  TCPSocket.open(@host, @port)
      str = s.gets
      s.close
      puts str
    rescue 
      @temp.push(" error")
      return ["error"]
    end
    tempdata = str.split("|")
    while tempdata.size > 0
      tempdata.shift
      dev = tempdata.shift.split("/")[2]
      tempdata.shift
      temp = tempdata.shift
      tempdata.shift
      @temp.push(" "+dev+" "+temp+" C")
    end
    return @temp
  end
end

class WmHddTemp
  def initialize(host_list = ["localhost"])
    @dock = DockApp.new("WmHddTemp")
    @text = DockApp::Text.new("", 9, 6)
    @dock.add(0, 0, @text)

    @hddtemp_list = []
    host_list.each do |host|
      @hddtemp_list.push(HddTemp.new(host))
    end

    @last_host_index = -1
    self.update
    self.show
    self.set_timer(@dock, @text)
  end

  def update()
    @hddtemp_list.each do |host|
      host.get()
    end
  end

  def show_old
    result_str = ""
    @hddtemp_list.each do |host|
      result_str += host.host+"\n"+host.temp.join("\n")+"\n"
    end
    @text.set_text(result_str)
  end

  def show
    @last_host_index += 1
    if @last_host_index > @hddtemp_list.size-1
      @last_host_index = 0
    end
    result_str = @hddtemp_list[@last_host_index].host+"\n"+
      @hddtemp_list[@last_host_index].temp.join("\n")+"\n"
    @text.set_text(result_str)
  end

  def set_timer(dock, text)
    # update timer
    t1 = DockApp::Timer.new(10000) do 
      self.update()
#      self.show()
    end
    t1.start

    t2 = DockApp::Timer.new(5000) do
      self.show()
    end
    t2.start
  end

  def run
    @dock.start
  end
end

wmhddtemp = WmHddTemp.new(ARGV).run

