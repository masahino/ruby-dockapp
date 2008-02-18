require 'test/unit'
require '../dockapp'
require 'thread'

class TestDockApp < Test::Unit::TestCase
  def setup
    puts "setup"
    @dockapp = DockApp.new("Test")
    @timer = DockApp::Timer.new(2000) do
      puts "destroy"
      @dockapp.destroy
      @timer.stop
    end
  end

  def test_name
    puts "test_name"
    assert_equal("Test", @dockapp.name)
  end
  
  def test_text_width
    puts "test_text_width"
    assert(@dockapp.text_width("hoge"))
  end

  def test_width
    puts "test_width"
    assert_equal(64, @dockapp.width)
  end

  def test_height
    puts "test_height"
    assert_equal(64, @dockapp.height)
  end

  def test_start
    puts "test_start"
    @timer.start
    @dockapp.start
  end
  def teardown
#    @dockapp.destroy
  end
end


