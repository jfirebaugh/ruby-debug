at_exit do
  puts "at_exit"
end

Debugger.debug_at_exit do
  puts "debug_at_exit"
end
