require "mkmf"

def add_define(name)
  $defs.push("-D#{name}")
end

have_type("rb_event_flag_t")
have_func("rb_errinfo")
have_func("rb_binding_new")

if RUBY_VERSION >= "1.9"
  require "ruby_core_source"

  add_define 'RUBY_19'

  hdrs = proc {
    have_struct_member("rb_method_entry_t", "body", "method.h")
    have_header("vm_core.h") and have_header("iseq.h") and have_header("insns.inc") and
    have_header("insns_info.inc") and have_header("eval_intern.h")
  }

  if RUBY_REVISION >= 26959 # rb_iseq_compile_with_option was added an argument filepath
    $CFLAGS << ' -DRB_ISEQ_COMPILE_6ARGS'
  end

  unless Ruby_core_source::create_makefile_with_core(hdrs, "ruby_debug")
    STDERR.print("Makefile creation failed\n")
    STDERR.print("*************************************************************\n\n")
    STDERR.print("  NOTE: For Ruby 1.9 installation instructions, please see:\n\n")
    STDERR.print("     http://wiki.github.com/mark-moseley/ruby-debug\n\n")
    STDERR.print("*************************************************************\n\n")
    exit(1)
  end
else
  add_define 'RUBY_18'
  create_makefile("ruby_debug")
end
