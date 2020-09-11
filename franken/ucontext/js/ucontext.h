#pragma once

#include <sys/types.h>

typedef void (*em_arg_callback_func)(void*);

typedef struct asyncify_data_s {
  void *stack_ptr;     /** Current position in the Asyncify stack (*not* the C stack) */
  void *stack_limit;   /** Where the Asyncify stack ends. */
  int rewind_id;       /** Interned ID of the rewind entry point; opaque to application. */
} asyncify_data_t;

typedef struct emscripten_fiber_s {
  void *stack_base;             /** Where the C stack starts (NOTE: grows down). */
  void *stack_limit;            /** Where the C stack ends. */
  void *stack_ptr;              /** Current position in the C stack. */
  em_arg_callback_func entry;   /** Function to call when resuming this context. If NULL, asyncify_data is used to rewind the call stack. */
  void *user_data;              /** Opaque pointer, passed as-is to the entry function. */
  asyncify_data_t asyncify_data;
} emscripten_fiber_t;

struct __ucontext_stack_t {
  void *ss_sp;
  int ss_flags;
  size_t ss_size;
};

typedef struct ucontext_t {
  emscripten_fiber_t fiber;
  unsigned long uc_flags;
  void *uc_link;
  struct __ucontext_stack_t uc_stack;
  struct __ucontext_stack_t as_stack;
} ucontext_t;
