#!/usr/bin/env ruby
require 'test/unit'

# begin require 'rubygems' rescue LoadError end
# require 'ruby-debug'; Debugger.start

# Test Debugger.load handles uncaught exceptions in the debugged program.
class TestBreakpoints < Test::Unit::TestCase

  @@SRC_DIR = File.join(Dir.pwd, File.dirname(__FILE__)) unless
    defined?(@@SRC_DIR)

  require File.join(@@SRC_DIR, 'helper')
  include TestHelper

  def test_basic
    testname='raise'
    Dir.chdir(@@SRC_DIR) do 
      script = File.join('data', testname + '.cmd')
# FIXME: Issue #6
#      assert_equal(true,
#                   run_debugger(testname,
#                                "--script #{script} -- ./raise.rb"))
    end
  end
end
