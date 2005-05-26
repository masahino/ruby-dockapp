require '../dockapp'

class DateAndTime

  def initialize 
    day_of_week = ["Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"]

    @dockapp = DockApp.new("Date and Time") # name
    @dockapp.openwindow 
    time_str = `date '+%H:%M:%S'`.chomp
    time = DockApp::Text.new(time_str, 8, 1, 0) # default string, width, height

    w = `date '+%w'`.to_i
    week = DockApp::Text.new(day_of_week[w], 3, 1, 0)

    date_str = `date '+%y/%m/%d'`.chomp
    date = DockApp::Text.new(date_str, 8, 1, 0)

    @dockapp.add(2, 5, date) # item, x, y
    @dockapp.add(2, 20, week)
    @dockapp.add(2, 35, time) # item, x, y
 
    time.click_callback do
      puts "clicked"
      if @t1.get_status == 1
        @t1.stop
      elsif @t1.get_status == 0
        @t1.start
      end
      GC.start
    end

    date.click_callback do
      `xcal`
      @dockapp.add(2, 20, week)
    end

    week.click_callback do
      @dockapp.delete(week)
    end

    @t1 = DockApp::Timer.new(100) do
      time_str = `date '+%H:%M:%S'`.chomp
      time.set_text(time_str)
    end

    @t2 = DockApp::Timer.new(60000) do
      w = `date '+%w'`.to_i
      week.set_text(day_of_week[w])
      date_str = `date '+%y/%m/%d'`.chomp
      date.set_text(date_str)
    end

  end

  def run
    @t1.start
    @t2.start
    @dockapp.start
  end

end

clock = DateAndTime.new()
clock.run
