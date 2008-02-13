require 'test/unit'
require '../dockapp'

class TestDockApp < Test::Unit::TestCase
  def setup
    @dockapp = DockApp.new("TestText")
    @dockapp.openwindow
  end

  def test_new
    text = DockApp::Text.new("test", 4, 1)
    @dockapp.add(10, 10, text)
    @dockapp.start
  end
end
