#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#include "solo5.h"

#ifndef EOS
#define EOS '\0'
#endif

#define SOLO5_INITIAL_MAXARGC 8

extern void solo5_init_mm(const struct solo5_start_info *);
extern int main(int, char **, char **);
int __franken_start_main(int (*)(int, char **, char **), int, char **, char **);
int argc = 0;
char **argv = NULL;
char **envp = NULL;

#define SOLO5_KERNEL_NAME "solo5"

#define SOLO5_ROOTFS_NAME "rootfs"

solo5_handle_t solo5_rootfs_handle = 0;
struct solo5_block_info *solo5_rootfs_info = NULL;

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
static char **build_args(const char *input)
{
  char *arg;
  char *copybuf;
  int squote = 0;
  int dquote = 0;
  int bsquote = 0;
  int argsc = 0;
  int maxargsc = 0;
  char **args = NULL;
  char **nargs;

  if (input == NULL)
    return NULL;

  copybuf = (char *)malloc(strlen(input) + 1);

  do {
    consume_space(&input);

    if ((maxargsc == 0) || (argsc >= (maxargsc - 1))) {
      if (args == NULL) {
        maxargsc = SOLO5_INITIAL_MAXARGC;
        nargs = (char **)malloc(maxargsc * sizeof(char *));
      } else {
        maxargsc *= 2;
        nargs = (char **)realloc(args, maxargsc * sizeof(char *));
      }
      args = nargs;
      args[argsc] = NULL;
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
    args[argsc] = strdup(copybuf);
    argsc++;
    args[argsc] = NULL;

    consume_space(&input);
  } while (*input != EOS);

  free(copybuf);

  return args;
}

static void build_params(char **args)
{
  char **arg = args;
  int envc = 0;
  int maxenvc = 0;
  int maxargc = 0;
  char **nargv;
  char **nenvp;

  if (arg == NULL)
    return;

  /* build envp */
  do {
    if ((maxenvc == 0) || (envc >= (maxenvc - 1))) {
      if (envp == NULL) {
        maxenvc = SOLO5_INITIAL_MAXARGC;
        nenvp = (char **)malloc(maxenvc * sizeof(char *));
      } else {
        maxenvc *= 2;
        nenvp = (char **)realloc(envp, maxenvc * sizeof(char *));
      }
      envp = nenvp;
      envp[envc] = NULL;
    }
    if (strncmp(*arg, "--", 2) == 0) {
      envp[envc] = NULL;
      arg++;
      break;
    }
    envp[envc] = strdup(*arg);
    envc++;
    envp[envc] = NULL;
    arg++;
  } while (*arg != NULL);

  /* build argv */
  if (argc == 0) {
    maxargc = SOLO5_INITIAL_MAXARGC;
    nargv = (char **)malloc(maxargc * sizeof(char *));
    argv = nargv;
    argv[argc] = SOLO5_KERNEL_NAME;
    argc++;
  }

  do {
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
    if (*arg == NULL) {
      argv[argc] = NULL;
      argc++;
      break;
    }
    argv[argc] = strdup(*arg);
    argc++;
    argv[argc] = NULL;
    arg++;
  } while (*arg != NULL);

  free(args);
}

static void parse_cmdline(const char *input)
{
  char **args;

  args = build_args(input);

  build_params(args);
}

int solo5_app_main(const struct solo5_start_info *info)
{
	solo5_init_mm(info);

    parse_cmdline(info->cmdline);

    solo5_rootfs_info = malloc(sizeof(struct solo5_block_info));

    solo5_block_acquire(SOLO5_ROOTFS_NAME, &solo5_rootfs_handle,
        solo5_rootfs_info);

	return __franken_start_main(main, argc, argv, envp);
}
