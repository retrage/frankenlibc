#include <ucontext.h>

extern void emscripten_fiber_init(
  emscripten_fiber_t *fiber,
  em_arg_callback_func entry_func,
  void *entry_func_arg,
  void *c_stack,
  size_t c_stack_size,
  void *asyncify_stack,
  size_t asyncify_stack_size
);

void makecontext(ucontext_t *ucp, void (*fn)(void), int argc, void *arg)
{
  emscripten_fiber_init(&ucp->fiber, (em_arg_callback_func)fn, arg,
                        ucp->uc_stack.ss_sp, ucp->uc_stack.ss_size,
                        ucp->as_stack.ss_sp, ucp->as_stack.ss_size);
}
