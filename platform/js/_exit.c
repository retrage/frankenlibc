#include <unistd.h>

typedef uint32_t __wasi_exitcode_t;

extern _Noreturn void __wasi_proc_exit(
  __wasi_exitcode_t rval 
) __attribute__((
  __import_module__("wasi_snapshot_preview1"),
  __import_name__("proc_exit")));

void _exit(int status)
{
  __wasi_proc_exit(status);
}
