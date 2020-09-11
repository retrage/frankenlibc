/* SPDX-License-Identifier: BSD-2-Clause */

extern void emscripten_run_script(const char *script);

extern char **environ;
extern int __main(int, char **, char **);
int __franken_start_main(int (*)(int, char **, char **), int, char **, char **);

int main(int argc, char **argv)
{
  emscripten_run_script("console.log('main #1')");

  char **envp = environ;

  return __franken_start_main(__main, argc, argv, envp);
}
