#!/usr/bin/ruby
$KCODE = 'e'
# xmmsでプレイ中のアルバムのイメージをamazonからダウンロードして表示する
# Dockアプリケーション

require 'jcode'
require 'dockapp'
require 'xmms'
require 'amazon/search'
require 'iconv'
load "#{ENV['HOME']}/.amazonid.conf"
# ~/.amazonid.confの書式は、以下のように
# $assoc_id = "アソシエイトID"
# $dev_token = "dev token"

$cover_dir="#{ENV['HOME']}/.cover/"

include Amazon::Search

# utf-8で検索しないとエラーになっちゃうので、
# そのためのもの
# http://namazu.org/~satoru/blog/archives/000005.html 
# で見つけた

class Iconv
  Preference = ["iso-2022-jp", 'euc-jp', 'utf-8',
    'shift_jis', 'windows-31j']
  def self.tou8 (str)
    return str if /\A[\r\n\t\x20-\x7e]*\Z/n.match(str) # us-ascii
    Preference.each {|name|
      begin
        return Iconv.conv("UTF-8", name, str)
      rescue Iconv::IllegalSequence
      end
    }
    raise 'unable to convert to UTF-8'
  end
end

class Cover

  def initialize 
    @dockapp = DockApp.new("Cover") # name
    @title_w = DockApp::Item.new(55, 12)
    @cover_w = DockApp::Item.new(40, 40) # default string, width, height
    
    @dockapp.openwindow

    @dockapp.add(0, 0, @title_w)
    @dockapp.add(10, 15, @cover_w) # item, x, y

    @r = Xmms::Remote.new
    @request = Request.new($dev_token, $assoc_id, "jp")

    @album = ""
    
    @timer = DockApp::Timer.new(500) do
      update_song()
    end
    @timer.start
    @cover_w.click_callback do
      if @artist != nil and @album != nil
        image_filename = $cover_dir+"/"+@artist+"_"+@album+".jpg"
        pid = fork do
          if File.exist?(image_filename)
            exec("display #{image_filename}")
          end
        end
      end
    end
  end
  
  def update_song()
    if @r.is_playing 
      path = @r.file
      now_title = @r.title
      now_file = File.basename(path)
      now_album = File.basename(File.dirname(path))
      now_artist = File.basename(File.dirname(File.dirname(path)))

      if now_title != @title
        @title = now_title
        @draw_str = " "*8 + @title
        #          title_w.clear
        #          title_w.drawtext(0, 10, @title)
        @title_pos = 0
      else
 #       if false
        text = @draw_str.split(//)[@title_pos, 8].join()
        if text.length > 8
          text = text[0, 8]
        end
        @title_w.clear
        @title_w.draw_string(1, 10, text)
        @title_pos += 1
        if @title_pos >= @draw_str.jlength
          @title_pos = 0
        end
#        end
      end
      
      if now_album != @album
        update_cover_image(now_album, now_artist)
      end
    end
  end
  
  def update_cover_image(now_album, now_artist)
    @album = now_album
    @artist = now_artist
    puts @album
    image_filename = $cover_dir+"/"+@artist+"_"+@album+".jpg"
    xpm_filename = "/tmp/cover.xpm"
    if FileTest.exist?(image_filename) == false
      response = @request.keyword_search(Iconv.tou8(@album), mode='music') do |product|
        @asin = product.asin
        image_url_medium = product.image_url_medium
        `wget -O #{image_filename} #{image_url_medium}`
        break
      end
    end
    `convert -resize 39x39 #{image_filename} #{xpm_filename}`
    @cover_w.set_pixmap(xpm_filename)
  end  
  def run
    @dockapp.start
  end
    
  end
sample = Cover.new
sample.run
