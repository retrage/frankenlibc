#if defined(__EMSCRIPTEN__)
#include "js/swapcontext.c"
#else
#error "Unknown architecture"
#endif
