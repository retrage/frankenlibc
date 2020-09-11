#include <ucontext.h>

extern void emscripten_fiber_swap(
  emscripten_fiber_t *old_fiber,
  emscripten_fiber_t *new_fiber
);

int swapcontext(ucontext_t *oucp, ucontext_t *ucp)
{
  emscripten_fiber_swap(&oucp->fiber, &ucp->fiber);

  return 0;
}
