extern void emscripten_run_script(const char *script);

int __main();
void _init() __attribute__((weak));
void _fini() __attribute__((weak));
int __libc_start_main(int (*)(int, char **, char **), int, char **);

int main(int argc, char **argv)
{
  emscripten_run_script("console.log('main #1')");

  int r = __libc_start_main(__main,)
  int r = __libc_start_main(__main, argc, argv, _init, _fini, 0);

  emscripten_run_script("console.log('main #2')");

  return r;
}
