#include <ruby.h>

/* 1.8 defines rb_event_t in node.h rather than rb_event_flag_t in ruby.h */
#ifndef HAVE_TYPE_RB_EVENT_FLAG_T
#include <node.h>
#define rb_event_flag_t rb_event_t
#endif

/* 1.8 does not provide rb_errinfo() or rb_set_errinfo() */
#ifndef HAVE_RB_ERRINFO
static VALUE rb_errinfo(void) { return ruby_errinfo; }
static void rb_set_errinfo(VALUE value) { ruby_errinfo = value; }
#endif

/* Context info */
enum ctx_stop_reason {CTX_STOP_NONE, CTX_STOP_STEP, CTX_STOP_BREAKPOINT, 
		      CTX_STOP_CATCHPOINT};

/* Context flags */
#define CTX_FL_SUSPEND      (1<<1)
#define CTX_FL_TRACING      (1<<2)
#define CTX_FL_SKIPPED      (1<<3)
#define CTX_FL_IGNORE       (1<<4)
#define CTX_FL_DEAD         (1<<5)
#define CTX_FL_WAS_RUNNING  (1<<6)
#define CTX_FL_ENABLE_BKPT  (1<<7)
#define CTX_FL_STEPPED      (1<<8)
#define CTX_FL_FORCE_MOVE   (1<<9)

#define CTX_FL_TEST(c,f)  ((c)->flags & (f))
#define CTX_FL_SET(c,f)   do { (c)->flags |= (f); } while (0)
#define CTX_FL_UNSET(c,f) do { (c)->flags &= ~(f); } while (0)

typedef struct {
    int argc;         /* Number of arguments a frame should have. */
    VALUE binding;
    ID id;
    ID orig_id;
    int line;
    const char * file;
    short dead;
    VALUE self;
    VALUE arg_ary;
    union {
        struct {
            struct FRAME *frame;
            struct SCOPE *scope;
            struct RVarmap *dyna_vars;
        } runtime;
        struct {
            VALUE args;
            VALUE locals;
	    VALUE arg_ary;
        } copy;
    } info;
} debug_frame_t;

typedef struct {
    VALUE thread_id;
    int thnum;
    int flags;
    enum ctx_stop_reason stop_reason;
    int stop_next;
    int dest_frame;
    int stop_line;
    int stop_frame;
    int stack_size;
    int stack_len;
    debug_frame_t *frames;
    const char * last_file;
    int last_line;
    VALUE breakpoint;
} debug_context_t;

/* variables in ruby_debug.c */
extern VALUE mDebugger;
extern VALUE rdebug_breakpoints;
extern VALUE rdebug_catchpoints;
extern VALUE rdebug_threads_tbl;

/* routines in ruby_debug.c */
extern int  filename_cmp(VALUE source, const char *file);

#define IS_STARTED  (rdebug_threads_tbl != Qnil)
static inline void
debug_check_started()
{
    if(!IS_STARTED)
    {
        rb_raise(rb_eRuntimeError, "Debugger.start is not called yet.");
    }
}

static inline int
classname_cmp(VALUE name, VALUE klass)
{
    VALUE class_name = (Qnil == name) ? rb_str_new2("main") : name;
    return (klass != Qnil 
	    && rb_str_cmp(class_name, rb_mod_name(klass)) == 0);
}

/* Breakpoint information */
enum bp_type {BP_POS_TYPE, BP_METHOD_TYPE};
enum hit_condition {HIT_COND_NONE, HIT_COND_GE, HIT_COND_EQ, HIT_COND_MOD};

typedef struct {
    int   id;
    enum bp_type type;
    VALUE source;
    union
    {
        int line;
        ID  mid;
    } pos;
    VALUE expr;
    VALUE enabled;
    int hit_count;
    int hit_value;
    enum hit_condition hit_condition;
} debug_breakpoint_t;

/* routines in breakpoint.c */
extern int   check_breakpoint_expression(VALUE breakpoint, VALUE binding);
extern int   check_breakpoint_hit_condition(VALUE breakpoint);
extern VALUE check_breakpoints_by_method(debug_context_t *debug_context, 
    VALUE klass, ID mid, VALUE self);
extern VALUE check_breakpoints_by_pos(debug_context_t *debug_context, 
    const char *file, int line);
extern VALUE create_breakpoint_from_args(int argc, VALUE *argv, int id);
extern VALUE context_breakpoint(VALUE self);
extern VALUE context_set_breakpoint(int argc, VALUE *argv, VALUE self);
extern VALUE rdebug_add_catchpoint(VALUE self, VALUE value);
extern VALUE debug_catchpoints(VALUE self);
extern VALUE rdebug_remove_breakpoint(VALUE self, VALUE id_value);

extern void Init_breakpoint();
