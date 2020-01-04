#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#include "solo5.h"

#ifndef EOS
#define EOS '\0'
#endif

#define SOLO5_INITIAL_MAXARGC 8

extern uintptr_t solo5_heap_start;
extern uintptr_t solo5_heap_end;
extern int main(int, char **, char **);
int __franken_start_main(int (*)(int, char **, char **), int, char **, char **);
int argc = 0;
char **argv = NULL;

static int is_whitespace(char c)
{
  switch (c) {
    case ' ':
    case '\t':
    case '\n':
    case '\v':
    case '\f':
    case '\r':
      return 1;
    default:
      return 0;
  }
}

static void consume_space(const char **input)
{
  while (is_whitespace(**input))
    (*input)++;
}

/* imported from https://github.com/gcc-mirror/gcc/blob/master/libiberty/argv.c */
static char **buildargv(const char *input)
{
  char *arg;
  char *copybuf;
  int squote = 0;
  int dquote = 0;
  int bsquote = 0;
  int maxargc = 0;
  char **nargv;

  if (input == NULL)
    return NULL;

  copybuf = (char *)malloc(strlen(input) + 1);

  do {
    consume_space(&input);

    if ((maxargc == 0) || (argc >= (maxargc - 1))) {
      if (argv == NULL) {
        maxargc = SOLO5_INITIAL_MAXARGC;
        nargv = (char **)malloc(maxargc * sizeof(char *));
      } else {
        maxargc *= 2;
        nargv = (char **)realloc(argv, maxargc * sizeof(char *));
      }
      argv = nargv;
      argv[argc] = NULL;
    }

    arg = copybuf;
    while (*input != EOS) {
      if (is_whitespace(*input) && !squote && !dquote && !bsquote) {
        break;
      } else {
        if (bsquote) {
          bsquote = 0;
          *arg++ = *input;
        } else if (*input == '\\') {
          bsquote = 1;
        } else if (squote) {
          if (*input == '\'') {
            squote = 0;
          } else {
            *arg++ = *input;
          }
        } else if (dquote) {
          if (*input == '"') {
            dquote = 0;
          } else {
            *arg++ = *input;
          }
        } else {
          if (*input == '\'') {
            squote = 1;
          } else if (*input == '"') {
            dquote = 1;
          } else {
            *arg++ = *input;
          }
        }
        input++;
      }
    }
    *arg = EOS;
    argv[argc] = strdup(copybuf);
    argc++;
    argv[argc] = NULL;

    consume_space(&input);
  } while (*input != EOS);

  free(copybuf);

  return argv;
}

int solo5_app_main(const struct solo5_start_info *info)
{
	char **envp;

	solo5_heap_start = info->heap_start;
    solo5_heap_end = info->heap_start + info->heap_size;

    argv = buildargv(info->cmdline);

    envp = argv + argc + 1;
	return __franken_start_main(main, argc, argv, envp);
}
