#!/usr/bin/ruby 

require 'dockapp.so'
require 'open-uri'
require 'net/imap'
require 'pp'
require 'yaml'

$default_rcfile = ENV['HOME']+'/.wmnotifierrc'

class NotifierAccount
  def get_num
  end
end

class LiveDoorNotifier < NotifierAccount
  attr_accessor :name, :user, :unread, :lastnum

  def initialize(name, user)
    @name = name
    @user = user
    @lastnum = 0
    @unread = 0
  end

  def update
    @lastnum = @unread
    @unread = 0
    begin
      open("http://rpc.reader.livedoor.com/notify?user=#{@user}") do |f|
        @unread = f.gets.split("|")[1].to_i
      end
    rescue
      @unread = -1
    end
    return @unread
  end

  def has_unread?
    if @unread > @lastnum
      return true
    end
    return false
  end
end

class ImapAccount < NotifierAccount
  attr_accessor :name, :lastnum, :unread

#imap = Net::IMAP.new("imap.ruby-lang.org")
#imap.authenticate("cram-md5", "shugo", "********")
#p imap.status("inbox", ["MESSAGES", "RECENT"])
#imap.logout
#imap.disconnect

  def initialize(name, server, user, password, folder = 'inbox')
    @name = name
    @server = server
    @user = user
    @password = password
    @lastnum = 0
    @unread = 0
    @folder = folder
  end
  
  def update
    begin
      @imap = Net::IMAP.new(@server)
      @imap.login(@user, @password)
      result = @imap.status(@folder, ["MESSAGES", "RECENT", "UNSEEN"])
      @imap.logout
      @imap.disconnect
      @lastnum = result["MESSAGES"]
      @unread = result["UNSEEN"]
    rescue
      @lastnum = -1
      @unread = -1
    end
  end

  def has_unread?
    if @unread > 0
      return true
    else
      return false
    end
  end

end

class Notifier
  def initialize(account_data)
    @accounts = []
    account_data.each do |a|
      case a[:type]
      when 'LiveDoorReader'
        @accounts.push(LiveDoorNotifier.new(a[:name], a[:user]))
      when 'IMAP'
        if a[:folder] != nil
          @accounts.push(ImapAccount.new(a[:name], a[:server], a[:user], a[:password], a[:folder]))
        else
          @accounts.push(ImapAccount.new(a[:name], a[:server], a[:user], a[:password]))
        end
      else
        exit
      end
    end
    @account_num = @accounts.size
    @notifier = DockApp.new("Notifier")
#    @window = DockApp::Text.new("", 9, @account_num)
    @window = DockApp::Item.new(56, @account_num * 11)

    @notifier.add((@notifier.width-@window.width)/2-4, 
                 (@notifier.height-@window.height)/2-3, @window)
    0.upto(@accounts.size-1) do |n|
      @window.drawLEDstring(1, 1+n*10, sprintf("%4s:", @accounts[n].name))
    end
    set_timer
  end

  def set_timer
    timer = DockApp::Timer.new(60000) do 
      self.update
    end
    timer.start
  end

  def update
    draw_string = ""
    n = 0
    @accounts.each do |account|
      account.update # あとでスレッドにしよう
      if account.has_unread?
        # blink
        for i in 0..3
          @window.drawLEDstring(31, 1+n*10, sprintf("%04d", account.unread), 1)
          sleep(0.3)
          @window.drawLEDstring(31, 1+n*10, sprintf("    "))
          sleep(0.3)
        end
        @window.drawLEDstring(31, 1+n*10, sprintf("%04d", account.unread), 1)
      else
        @window.drawLEDstring(31, 1+n*10, sprintf("%04d", account.lastnum))
      end
      n += 1
    end
  end

  def start
    @notifier.start
  end
end

account_data = YAML::load_file($default_rcfile)
n = Notifier.new(account_data)
n.start
