#include "arg.h"
#include "libyam/config.h"
#include "libyam/log.h"
#include "libyam/sink.h"
#include <argtable2.h>

#define YAM_MAX_SOURCE 1028

struct arg_lit *verb = NULL;
struct arg_lit *help = NULL;
struct arg_lit *version = NULL;

struct arg_str *sources = NULL;
struct arg_str *sink = NULL;
struct arg_str *drain = NULL;

struct arg_lit *total_read = NULL;

// arg end stores errors
struct arg_end *end = NULL;

#define yam_argtable                                                           \
  { help, version, verb, sink, drain, sources, total_read, end, }

struct yam_config yam_args_to_config(int argc, char **argv) {
  help = arg_litn(NULL, "help", 0, 1, "display this help and exit");
  version = arg_litn(NULL, "version", 0, 1, "display version info and exit");
  verb = arg_litn("v", "verbose", 0, YAM_LOG_LEVEL_DBG, "verbose output");
  sink = arg_str0("s", "sink", "sink name",
                  "Select which converter to use. (" YAM_SINK_C_CHAR_ARRAY_STR
                  "[:variable name])");
  total_read = arg_lit0("t", "total", "Output total amount read to stdout");
  drain = arg_str0("o", "output", "FILE", "Select an output file");
  sources = arg_strn(
      NULL, NULL, "INPUT", 0, YAM_MAX_SOURCE,
      "Provide an input source. Valid sources: [path], " YAM_PREFIX_FILE
      "[:path], " YAM_PREFIX_STRING "[:string], " YAM_PREFIX_HEX_STRING
      "[:hex string], " YAM_PREFIX_PADDING "[amount:char:step]");
  end = arg_end(20);

  void *argtable[] = yam_argtable;

  struct yam_config cfg = yam_config_init();

  // output params
  char progname[] = "yam";
  char short_desc[] =
      "Yam - A simple mapper of binary inputs to another representation for "
      "quickly getting data into a usable format.";

  // version info
  int version_major = 0;
  int version_minor = 0;
  int version_patch = 1;

  int nerrors = arg_parse(argc, argv, argtable);
  int exitcode = 0;

  if (help->count > 0) {
    printf("Usage: %s", progname);
    arg_print_syntax(stdout, argtable, "\n");
    printf("%s\n\n", short_desc);
    arg_print_glossary(stdout, argtable, "  %-25s %s\n");
    goto exit;
  }

  if (version->count) {
    printf("%s version %d.%d.%d\n", progname, version_major, version_minor,
           version_patch);
    goto exit;
  }

  if (verb->count > 0) {
    cfg.verbose = verb->count;
  } else {
    yam_log_init(YAM_LOG_LEVEL_ERROR);
  }

  if (nerrors > 0) {
    arg_print_errors(stdout, end, progname);
    printf("Try '%s --help' for more information.\n", progname);
    exitcode = 1;
    goto exit;
  }

  // map args to cfg data here
  if (sources->count) {
    cfg.source_exprs = sources->sval;
    cfg.source_exprs_len = sources->count;
  }

  if (sink->count) {
    cfg.sink_expr = sink->sval[0];
  }

  if (drain->count) {
    cfg.drain_expr = drain->sval[0];
  }

  cfg.print_total_read = total_read->count > 0;

  return cfg;
exit:
  yam_args_free();
  exit(exitcode);
}

void yam_args_free(void) {
  void *argtable[] = yam_argtable;
  arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}
