#!/usr/bin/env rake
# -*- Ruby -*-
require 'rubygems'
require 'rubygems/package_task'
require 'rdoc/task'
require 'rake/testtask'
require 'rake/extensiontask'
require 'rake/javaextensiontask'

$:.push File.expand_path("../lib", __FILE__)
require "ruby-debug-base/version"

SO_NAME = "ruby_debug.so"
ROOT_DIR = File.dirname(__FILE__)

# ------- Default Package ----------
COMMON_FILES = FileList[
  'AUTHORS',
  'CHANGES',
  'LICENSE',
  'README',
  'Rakefile',
]                        

CLI_TEST_FILE_LIST = FileList['test/cli/commands/unit/*.rb',
                              'test/cli/commands/*_test.rb', 
                              'test/cli/**/*_test.rb', 
                              'test/test-*.rb'] 
CLI_FILES = COMMON_FILES + FileList[
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
  'rdbg.rb',
   CLI_TEST_FILE_LIST
]

BASE_TEST_FILE_LIST = FileList['test/base/*.rb']

BASE_FILES = COMMON_FILES + FileList[
  'ext/breakpoint.c',
  'ext/extconf.rb',
  'ext/ruby_debug.c',
  'ext/ruby_debug.h',
  'ext/win32/*',
  'lib/ruby-debug-base.rb',
  'lib/ruby-debug-base/version.rb',
  BASE_TEST_FILE_LIST,
]

ext = File.join(ROOT_DIR, 'ext')

desc "Test everything."
Rake::TestTask.new(:test) do |t|
  t.libs += %W(#{ROOT_DIR}/lib #{ROOT_DIR}/cli)
  t.libs << ext if File.exist?(ext)
  t.test_files = CLI_TEST_FILE_LIST
  t.options = '--verbose' if $VERBOSE
  t.ruby_opts << "--debug" if defined?(JRUBY_VERSION)
end

task :test => :test_base if File.exist?(ext)

desc "Test ruby-debug-base."
Rake::TestTask.new(:test_base) do |t|
  t.libs += ['./ext', './lib']
  t.test_files = FileList[BASE_TEST_FILE_LIST]
  t.options = '--verbose' if $VERBOSE
  t.ruby_opts << "--debug" if defined?(JRUBY_VERSION)
end

if defined?(JRUBY_VERSION)
  task :test_base => 'jruby:compile:java'
else
  task :test_base => :compile
end

desc "Test everything - same as test."
task :check => :test

desc "Create a GNU-style ChangeLog via svn2cl"
task :ChangeLog do
  system('git log --pretty --numstat --summary     | git2cl >     ChangeLog')
  system('git log --pretty --numstat --summary ext | git2cl > ext/ChangeLog')
  system('git log --pretty --numstat --summary lib | git2cl > lib/ChangeLog')
end

# Base GEM Specification
base_spec = Gem::Specification.new do |spec|
  spec.name = "ruby-debug-base"
  
  spec.homepage = "http://rubyforge.org/projects/ruby-debug/"
  spec.summary = "Fast Ruby debugger - core component"
  spec.description = <<-EOF
ruby-debug is a fast implementation of the standard Ruby debugger debug.rb.
It is implemented by utilizing a new Ruby C API hook. The core component 
provides support that front-ends can build on. It provides breakpoint 
handling, bindings for stack frames among other things.
EOF

  spec.version = Debugger::VERSION

  spec.author = "Kent Sibilev"
  spec.email = "ksibilev@yahoo.com"
  spec.platform = Gem::Platform::RUBY
  spec.require_path = "lib"
  spec.extensions = ["ext/extconf.rb"]
  spec.files = BASE_FILES.to_a  

  spec.required_ruby_version = '>= 1.8.2'
  spec.date = Time.now
  spec.rubyforge_project = 'ruby-debug'
  spec.add_dependency('linecache', '>= 0.3')
  spec.add_development_dependency('rake-compiler')

  spec.test_files = FileList[BASE_TEST_FILE_LIST]
  
  # rdoc
  spec.has_rdoc = true
  spec.extra_rdoc_files = ['README', 'ext/ruby_debug.c']
end

cli_spec = Gem::Specification.new do |spec|
  spec.name = "ruby-debug"
  
  spec.homepage = "http://rubyforge.org/projects/ruby-debug/"
  spec.summary = "Command line interface (CLI) for ruby-debug-base"
  spec.description = <<-EOF
A generic command line interface for ruby-debug.
EOF

  spec.version = Debugger::VERSION

  spec.author = "Kent Sibilev"
  spec.email = "ksibilev@yahoo.com"
  spec.platform = Gem::Platform::RUBY
  spec.require_path = "cli"
  spec.bindir = "bin"
  spec.executables = ["rdebug"]
  spec.files = CLI_FILES.to_a

  spec.required_ruby_version = '>= 1.8.2'
  spec.date = Time.now
  spec.rubyforge_project = 'ruby-debug'
  spec.add_dependency('columnize', '>= 0.1')
  spec.add_dependency('ruby-debug-base', "~> #{Debugger::VERSION}.0")
  
  # FIXME: work out operational logistics for this
  # spec.test_files = FileList[CLI_TEST_FILE_LIST]

  # rdoc
  spec.has_rdoc = true
  spec.extra_rdoc_files = ['README']
end

# Rake task to build the default package
Gem::PackageTask.new(base_spec) do |pkg|
  pkg.need_tar = true
end
Gem::PackageTask.new(cli_spec) do |pkg|
  pkg.need_tar = true
end

Rake::ExtensionTask.new('ruby_debug', base_spec) do |t|
  t.ext_dir = "ext"
end

task :default => :test

# Windows specification
win_spec = base_spec.clone
win_spec.extensions = []
## win_spec.platform = Gem::Platform::WIN32 # deprecated
win_spec.platform = 'mswin32'
win_spec.files += ["lib/#{SO_NAME}"]

desc "Create Windows Gem"
task :win32_gem do
  # Copy the win32 extension the top level directory
  current_dir = File.expand_path(File.dirname(__FILE__))
  source = File.join(current_dir, "ext", "win32", SO_NAME)
  target = File.join(current_dir, "lib", SO_NAME)
  cp(source, target)

  # Create the gem, then move it to pkg.
  Gem::Builder.new(win_spec).build
  gem_file = "#{win_spec.name}-#{win_spec.version}-#{win_spec.platform}.gem"
  mv(gem_file, "pkg/#{gem_file}")

  # Remove win extension from top level directory.
  rm(target)
end

desc "Publish ruby-debug to RubyForge."
task :publish do 
  require 'rake/contrib/sshpublisher'
  
  # Get ruby-debug path.
  ruby_debug_path = File.expand_path(File.dirname(__FILE__))

  Rake::SshDirPublisher.new("kent@rubyforge.org",
        "/var/www/gforge-projects/ruby-debug", ruby_debug_path)
end

desc "Remove built files"
task :clean do
  cd "ext" do
    if File.exists?("Makefile")
      sh "make clean"
      rm  "Makefile"
    end
    derived_files = Dir.glob(".o") + Dir.glob("*.so")
    rm derived_files unless derived_files.empty?
  end
  rm 'lib/ruby_debug.jar' if File.exists?("lib/ruby_debug.jar")
end

# ---------  RDoc Documentation ------
desc "Generate rdoc documentation"
RDoc::Task.new("rdoc") do |rdoc|
  rdoc.rdoc_dir = 'doc/rdoc'
  rdoc.title    = "ruby-debug"
  # Show source inline with line numbers
  rdoc.options << "--inline-source" << "--line-numbers"
  # Make the readme file the start page for the generated html
  rdoc.options << '--main' << 'README'
  rdoc.rdoc_files.include('bin/**/*',
                          'cli/ruby-debug/commands/*.rb',
                          'cli/ruby-debug/*.rb',
                          'lib/**/*.rb',
                          'ext/**/ruby_debug.c',
                          'README',
                          'LICENSE')
end

desc "Publish the release files to RubyForge."
task :rubyforge_upload do
  `rubyforge login`
  release_command = "rubyforge add_release #{PKG_NAME} #{PKG_NAME} '#{PKG_NAME}-#{PKG_VERSION}' pkg/#{PKG_NAME}-#{PKG_VERSION}.gem"
  puts release_command
  system(release_command)
end

PKG_NAME      = 'ruby-debug'
desc "Publish the release files to RubyForge."
task :rubyforge_upload do
  `rubyforge login`
  for pkg_name in ['ruby-debug', 'ruby-debug-base'] do
    pkg_file_name = "#{pkg_name}-#{pkg_version}"
    release_command = "rubyforge add_release ruby-debug #{pkg_name} '#{pkg_file_name}' pkg/#{pkg_file_name}.gem"
    puts release_command
    system(release_command)
  end
end

def install_gem(spec, *opts)
  args = ['gem', 'install', "pkg/#{spec.name}-#{spec.version}.gem"] + opts
  args.unshift 'sudo' unless 0 == Process.uid || ENV['rvm_path']
  system(*args)
end

desc 'Install locally'
task :install => :package do
  Dir.chdir(File::dirname(__FILE__)) do
    # ri and rdoc take lots of time
    install_gem(base_spec, '--no-ri', '--no-rdoc')
    install_gem(cli_spec, '--no-ri', '--no-rdoc')
  end
end    

task :install_full => :package do
  Dir.chdir(File::dirname(__FILE__)) do
    install_gem(base_spec)
    install_gem(cli_spec)
  end
end    

namespace :jruby do
  jruby_spec = base_spec.clone
  jruby_spec.platform   = "java"
  jruby_spec.files      = jruby_spec.files.reject {|f| f =~ /^ext/ }
  jruby_spec.files     += ['lib/ruby_debug.jar']
  jruby_spec.extensions = []

  Gem::PackageTask.new(jruby_spec) {}

  Rake::JavaExtensionTask.new('ruby_debug') do |t|
    t.ext_dir = "src"
  end
end
