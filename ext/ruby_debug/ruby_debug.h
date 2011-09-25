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
#define CTX_FL_CATCHING     (1<<10)

#define CTX_FL_TEST(c,f)  ((c)->flags & (f))
#define CTX_FL_SET(c,f)   do { (c)->flags |= (f); } while (0)
#define CTX_FL_UNSET(c,f) do { (c)->flags &= ~(f); } while (0)

typedef struct {
    struct iseq_catch_table_entry tmp_catch_table;
    struct iseq_catch_table_entry *old_catch_table;
    int old_catch_table_size;
    VALUE mod_name;
    VALUE errinfo;
} debug_catch_t;

typedef struct {
    struct rb_iseq_struct *iseq;
    struct iseq_catch_table_entry *catch_table;
    int catch_table_size;
} iseq_catch_t;

typedef struct {
    VALUE thread_id;
    int thnum;
    int flags;
    enum ctx_stop_reason stop_reason;
    int stop_next;
    int dest_frame;
    int stop_line;
    int stop_frame;
    int stack_len;
    const char * last_file;
    int last_line;
    VALUE breakpoint;
    debug_catch_t catch_table;
    VALUE saved_jump_ins[2];
    rb_control_frame_t *jump_cfp;
    VALUE *jump_pc;
    iseq_catch_t *old_iseq_catch;
    VALUE last_exception;
    volatile int thread_pause;
//
    rb_control_frame_t *start_cfp;
    rb_control_frame_t *cur_cfp;
    rb_control_frame_t *top_cfp;
    rb_control_frame_t *catch_cfp;
    rb_control_frame_t *frames;
//
    struct RData catch_rdata;
    struct rb_iseq_struct catch_iseq;
    struct iseq_insn_info_entry catch_info_entry;
    struct RNode catch_cref_stack;
    VALUE iseq_insn[4];
//
    rb_control_frame_t **cfp;
    int cfp_count;
} debug_context_t;

/* variables in ruby_debug.c */
extern VALUE mDebugger;
extern VALUE rdebug_breakpoints;
extern VALUE rdebug_catchpoints;
extern VALUE rdebug_threads_tbl;

/* routines in ruby_debug.c */
extern int  filename_cmp(VALUE source, const char *file);

static inline int
classname_cmp(VALUE name, VALUE klass)
{
    VALUE mod_name;
    VALUE class_name = (Qnil == name) ? rb_str_new2("main") : name;
    if (klass == Qnil) return(0);
    mod_name = rb_mod_name(klass);
    return (mod_name != Qnil 
	    && rb_str_cmp(class_name, mod_name) == 0);
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
