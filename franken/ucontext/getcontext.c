#include <ucontext.h>

#if 0
#ifdef __EMSCRIPTEN__
extern void emscripten_fiber_init_from_current_context(
  emscripten_fiber_t *fiber,
  void *asyncify_stack,
  size_t asyncify_stack_size
);
#endif
#endif

int
getcontext(ucontext_t *ucp)
{
#if 0
#ifdef __EMSCRIPTEN__
	emscripten_fiber_init_from_current_context(&ucp->fiber,
        &ucp->asyncify_stack, sizeof(ucp->asyncify_stack));
#endif
#endif
	return 0;
}
