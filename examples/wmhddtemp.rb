#!/usr/bin/env ruby

require 'dockapp'
require 'socket'

host_list = ["panda", "bear", "tiger"]

class HddTemp
  attr_accessor :host

  def initialize(host, port = 7634)
    @host = host
    @port = port
  end
  def get()
    result = []
    begin
      str =  TCPSocket.open(@host, @port).gets
    rescue
      return "error"
    end
    tempdata = str.split("|")
    while tempdata.size > 0
      tempdata.shift
      dev = tempdata.shift.split("/")[2]
      tempdata.shift
      temp = tempdata.shift
      tempdata.shift
      result.push(" "+dev+" "+temp+" C")
    end
    return result
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
    self.set_timer(@dock, @text, @hddtemp_list)
  end

  def get_data(host_list)
    data = []
    host_list.each do |host|
      data.push(host.host())
      data.push(host.get())
    end
    return data
  end

  def set_timer(dock, text, hddtemp_list)
    t1 = DockApp::Timer.new(10000) do 
      @data = self.get_data(hddtemp_list)
      @text.set_text(@data.join("\n").chomp)
    end
    t1.start
  end

  def run
    @dock.start
  end
end

wmhddtemp = WmHddTemp.new(ARGV).run

