#!/usr/bin/env ruby
require File.expand_path("../helper", __FILE__)

class TestAtExit < Test::Unit::TestCase
  include TestHelper

  def test_at_exit_breakpoints
    testname = 'at-exit'
    script = File.join('data', testname + '.cmd')
    assert(run_debugger(testname, "--script #{script} -- ./example/at-exit.rb"))
  end

  def test_debug_at_exit
    testname = 'debug-at-exit'
    script = File.join('data', testname + '.cmd')
    assert(run_debugger(testname, "--script #{script} -- ./example/at-exit.rb"))
  end
end
