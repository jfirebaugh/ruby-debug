Gem::Specification.new do |s|
  s.name = "ruby-debug-base"

  s.homepage = "http://rubyforge.org/projects/ruby-debug/"
  s.summary = "Fast Ruby debugger - core component"
  s.description = <<-EOF
ruby-debug is a fast implementation of the standard Ruby debugger debug.rb.
It is implemented by utilizing a new Ruby C API hook. The core component 
provides support that front-ends can build on. It provides breakpoint 
handling, bindings for stack frames among other things.
EOF

  s.version = Debugger::VERSION

  s.author = "Kent Sibilev"
  s.email = "ksibilev@yahoo.com"
  s.platform = Gem::Platform::RUBY
  s.require_path = "lib"
  s.extensions = ["ext/extconf.rb"]
  s.files = FileList[
      'AUTHORS',
      'CHANGES',
      'LICENSE',
      'README',
      'Rakefile',
      'ext/breakpoint.c',
      'ext/extconf.rb',
      'ext/ruby_debug.c',
      'ext/ruby_debug.h',
      'ext/win32/*',
      'lib/ruby-debug-base.rb',
      'lib/ruby-debug/version.rb']

  s.required_ruby_version = '>= 1.8.2'
  s.date = Time.now
  s.rubyforge_project = 'ruby-debug'
  s.add_dependency('linecache', '>= 0.3')
  s.add_development_dependency('rake-compiler')

  s.test_files = FileList['test/base/*.rb']
  
  # rdoc
  s.has_rdoc = true
  s.extra_rdoc_files = ['README', 'ext/ruby_debug.c']
end
