Gem::Specification.new do |s|
  s.name = "ruby-debug"

  s.homepage = "http://rubyforge.org/projects/ruby-debug/"
  s.summary = "Command line interface (CLI) for ruby-debug-base"
  s.description = <<-EOF
A generic command line interface for ruby-debug.
  EOF

  s.version = Debugger::VERSION

  s.author = "Kent Sibilev"
  s.email = "ksibilev@yahoo.com"
  s.platform = Gem::Platform::RUBY
  s.require_path = "cli"
  s.bindir = "bin"
  s.executables = ["rdebug"]
  s.files = FileList[
      'AUTHORS',
      'CHANGES',
      'LICENSE',
      'README',
      'Rakefile',
      "cli/**/*",
      'ChangeLog',
      'bin/*',
      'doc/rdebug.1',
      'test/rdebug-save.1',
      'test/**/data/*.cmd',
      'test/**/data/*.right',
      'test/**/example/*.rb',
      'test/config.yaml',
      'test/**/*.rb',
      'rdbg.rb']

  s.required_ruby_version = '>= 1.8.2'
  s.date = Time.now
  s.rubyforge_project = 'ruby-debug'
  s.add_dependency('columnize', '>= 0.1')
  s.add_dependency('ruby-debug-base', "~> #{Debugger::VERSION}.0")

  s.test_files = FileList['test/cli/commands/unit/*.rb',
                             'test/cli/commands/*_test.rb',
                             'test/cli/**/*_test.rb',
                             'test/test-*.rb']

  # rdoc
  s.has_rdoc = true
  s.extra_rdoc_files = ['README']
end
