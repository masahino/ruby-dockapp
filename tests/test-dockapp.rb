require 'test/unit'
require '../dockapp'


class TestDockApp < Test::Unit::TestCase
  def setup
    @dockapp = DockApp.new("Test")
    @dockapp.openwindow
  end

  def test_name
    assert_equal("Test", @dockapp.name)
  end
  
  def test_text_width
    assert(@dockapp.text_width("hoge"))
  end

  def test_width
    assert_equal(64, @dockapp.width)
  end

  def test_height
    assert_equal(64, @dockapp.height)
  end

  def test_start
    @dockapp.start
  end
  def teardown
#    @dockapp.destroy
  end
end


